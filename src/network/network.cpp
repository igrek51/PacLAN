#include "network.h"
#include "../config.h"
#include "../log.h"
#include "../app.h"
#include "../system.h"
#include "../utils.h"

#include <arpa/inet.h>
#include <netdb.h>

Network::Network() : ContinuousThread(150) {
    App::network = this;
    error = false;
    server = false;
    client = false;
    recv_buffer = new char[Config::buffer_size];
    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 500;
    max_socket = 0;
    Log::info("Wątek interfejsu sieci zainicjalizowany.");
    init = true;
}

Network::~Network() {
    //zamykanie socketów
    if (server || client) {
        disconnect_socket(0);
    }
    sockets.clear();
    delete[] recv_buffer;
    recv_buffers.clear();
    recv_packets.clear();
    tasks.clear();
    Log::info("Zamykanie wątku interfejsu sieci...");
}

void Network::runLoop() {
    //wykonanie zakolejkowanych zadań
    if (tasks.size() > 0) {
        task_exec(tasks.at(0));
        delete tasks.at(0);
        tasks.erase(tasks.begin());
    }

    //odbieranie i obsługa eventów
    if (sockets.size() > 0) {

        //clear the socket set
        FD_ZERO(&read_sockets);

        //add master socket to set
        int max_sd = 0;
        if (server) {
            int server_socket = sockets.at(0);
            FD_SET(server_socket, &read_sockets);
            max_sd = server_socket;
            //add child sockets to set
            for (unsigned int i = 0; i < sockets.size(); i++) {
                //socket descriptor
                int sd = sockets[i];

                //if valid socket descriptor then add to read list
                if (sd > 0)
                    FD_SET(sd, &read_sockets);

                //highest file descriptor number, need it for the select function
                if (sd > max_sd)
                    max_sd = sd;
            }
        } else if (client) {
            int client_socket = sockets.at(0);
            FD_SET(client_socket, &read_sockets);
            max_sd = client_socket;
        }

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        int activity = select(max_sd + 1, &read_sockets, NULL, NULL, &select_timeout);

        if (activity < 0) {
            network_error("select criticalError");
        }

        //If something happened on the master socket , then its an incoming connection
        if (server) {
            int server_socket = sockets.at(0);
            if (FD_ISSET(server_socket, &read_sockets)) {

                Log::info("FD_accpet: 0");

                fd_accept();
            }
        }

        //else its some IO operation on some other socket :)
        for (unsigned i = 0; i < sockets.size(); i++) {
            if (server && i == 0) {
                continue; //socket serwera tylko do nawiązywania nowych połączeń
            }

            int sd = sockets.at(i);

            if (FD_ISSET(sd, &read_sockets)) {

                Log::info("FD_READ (| FD_close): " + itos(i));

                read_packet(i);
            }
        }

    }
    sleep_ms(2); //zmniejszenie zużycia procesora
}

void Network::network_error(string e) {
    error = true;
    stringstream ss;
    ss << "Błąd sieci: " << e;
    Log::error(ss.str());
    console_out(ss.str());
}

void Network::console_out(string e) {
    App::game_engine->cmd_output(e);
}

string Network::get_myip() {
    char* hostname = new char[128];
    gethostname(hostname, 128);
    hostent* hp = gethostbyname(hostname);
    delete[] hostname;
    sockaddr_in source;
    if (hp) {
        memcpy(&(source.sin_addr), hp->h_addr, hp->h_length);
        return inet_ntoa(source.sin_addr);
    }
    return "";
}

string Network::get_myname() {
    char* hostname = new char[128];
    gethostname(hostname, 128);
    string hname = hostname;
    delete[] hostname;
    return hname;
}

string Network::get_client_ip(int sindex) {
    if (sindex < 0 || sindex >= (int) sockets.size())
        return "";
    sockaddr_in client_info;
    socklen_t client_info_size = sizeof(client_info);
    memset(&client_info, 0, sizeof(client_info));
    getpeername(sockets.at(sindex), (sockaddr*) &client_info, &client_info_size);
    return inet_ntoa(client_info.sin_addr);
}

bool Network::open_server_socket() {
    if (client) {
        network_error("Otwarte jest już połączenie z hostem, zamknij je.");
        return false;
    }
    if (server) {
        console_out("Restartuje serwer...");
        disconnect_socket(0);
    } else {
        console_out("Otwieram serwer...");
    }
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == 0) {
        network_error("openning server socket failed");
        return false;
    }

    int opt = 1;
    //allow multiple connections
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        network_error("failed setting server socket options");
        return false;
    }

    sockaddr_in serverInfo;
    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(Config::port);

    if (bind(server_socket, (struct sockaddr*) &serverInfo, sizeof(serverInfo)) < 0) {
        network_error("open_server_socket: błąd utworzenia serwera");
        return false;
    }
    if (listen(server_socket, Config::max_clients) < 0) {
        network_error("open_server_socket: błąd nasłuchiwania");
        return false;
    }
    sockets.push_back(server_socket);
    recv_buffers.push_back(new vector<char>);
    recv_packets.push_back(new vector<string>);
    server = true;
    updateSockets();
    console_out("Połączenie serwera otwarte.");
    return true;
}

bool Network::connect_socket(string ip) {
    if (server) {
        network_error("Otwarty jest już serwer, zamknij połączenie serwera.");
        return false;
    }
    if (client) {
        console_out("Restart połączenia z serwerem...");
        disconnect_socket(0);
    } else {
        console_out("Szukanie hosta: " + ip + " ...");
    }
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == 0) {
        network_error("creating client socket failed");
        return false;
    }
    sockaddr_in clientInfo;
    memset(&clientInfo, 0, sizeof(clientInfo));
    clientInfo.sin_family = AF_INET;
    if (ip == "0") {
        clientInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        if (inet_addr(ip.c_str()) == INADDR_NONE) {
            hostent* hostEntry = gethostbyname(ip.c_str());
            if (hostEntry == nullptr) {
                network_error("Nie znaleziono hosta.");
                return false;
            }
            memcpy(&clientInfo.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
        } else {
            clientInfo.sin_addr.s_addr = inet_addr(ip.c_str());
        }
    }
    ip = inet_ntoa(clientInfo.sin_addr);
    clientInfo.sin_port = htons(Config::port);
    console_out("Łączenie do: " + ip + " ...");
    if (connect(client_socket, (struct sockaddr*) &clientInfo, sizeof(struct sockaddr)) < 0) {
        network_error("Błąd połączenia do hosta");
        return false;
    }
    sockets.push_back(client_socket);
    recv_buffers.push_back(new vector<char>);
    recv_packets.push_back(new vector<string>);
    int sindex = sockets.size() - 1;
    client = true;
    stringstream ss2;
    ss2 << "Połączono z: " << inet_ntoa(clientInfo.sin_addr);
    console_out(ss2.str());
    updateSockets();
    onConnectedToServer(sindex);
    return true;
}

void Network::remove_socket(int sindex) {
    string ip;
    if (sindex >= 1) {
        ip = get_client_ip(sindex);
    }
    //zamykanie socketu
    if (!closeSocket(sockets.at(sindex))) {
        network_error("closesocket: błąd zamykania socketu");
        return;
    }
    sockets.erase(sockets.begin() + sindex);
    //usunięcie bufora odbioru danych
    recv_buffers.at(sindex)->clear();
    recv_buffers.erase(recv_buffers.begin() + sindex);
    recv_packets.at(sindex)->clear();
    recv_packets.erase(recv_packets.begin() + sindex);
    updateSockets();
    if (sindex >= 1) {
        console_out("Klient rozłączony: " + ip);
    }
}

void Network::disconnect_socket(int sindex) {
    if (sindex < 0 || sindex >= (int) sockets.size()) return;
    if (client) { //zamknięcie klienta
        if (sindex == 0) {
            remove_socket(sindex);
            client = false;
            console_out("Zamknięto połączenie z serwerem.");
        }
    } else if (server) {
        if (sindex == 0) { //zamknięcie całego serwera
            //odłączenie klientów podłączonych do serwera
            while (sockets.size() > 1) {
                remove_socket(1);
            }
            remove_socket(sindex);
            server = false;
            console_out("Zamknięto serwer.");
        } else { //rozłączenie klienta od serwera
            remove_socket(sindex);
        }
    }
}

bool Network::closeSocket(int socket) {
    if (socket >= 0) {
        // first clear any errors, which can cause close to fail
        int err = 1;
        socklen_t len = sizeof err;
        if (-1 == getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*) &err, &len))
            return false;
        if (shutdown(socket, SHUT_RDWR) < 0) // secondly, terminate the 'reliable' delivery
        if (close(socket) < 0) // finally call close()
            return false;
        Log::info("Zamknięto socket.");
    }
    return true;
}

void Network::fd_accept() {
    console_out("Serwer: Żądanie nawiązania połączenia");
    if (server) {
        sockaddr_in clientInfo;
        socklen_t iAddrLen = sizeof(clientInfo);
        int new_client = accept(sockets.at(0), (struct sockaddr*) &clientInfo, &iAddrLen);
        if (new_client < 0) {
            network_error("Błąd przy nawiązywaniu połączenia");
            return;
        }
        sockets.push_back(new_client);
        recv_buffers.push_back(new vector<char>);
        recv_packets.push_back(new vector<string>);
        updateSockets();
        stringstream ss;
        ss << "Nawiązano połączenie z klientem: " << inet_ntoa(clientInfo.sin_addr);
        console_out(ss.str());
    }
}

void Network::fd_close(int sindex) {
    stringstream ss;
    ss << "Zamknięcie połączenia: socket " << sindex;
    console_out(ss.str());
    if (client && sindex == 0) { //zamknięto serwer
        App::game_engine->cmd_on = true;
    }
    disconnect_socket(sindex);
    if (server) { //zamknięto klienta
        ss_clear(ss);
        ss << "201 " << sindex;
        recv_packets.at(0)->push_back(ss.str());
    }
}

bool Network::send_packet(int sindex, char* msg, int len) {
    if (sindex < 0 || sindex >= (int) sockets.size()) {
        network_error("Numer socketu spoza zakresu");
        return false;
    }
    if (len == 0)
        return true;
    if (!client && !server) {
        network_error("Niepołączony");
        return false;
    }
    char* msg2 = new char[len];
    memcpy(msg2, msg, len);
    //podział pakietów
    int max_send_size = Config::buffer_size;
    if (max_send_size > 0) {
        if (len > max_send_size) {
            bool wynik1 = send_packet(sockets.at(sindex), msg2, max_send_size);
            bool wynik2 = send_packet(sockets.at(sindex), msg2 + max_send_size,
                                      len - max_send_size);
            delete[] msg2;
            return wynik1 & wynik2;
        }
    }
    int attempts = 1;
    int retval = send(sockets.at(sindex), msg2, len, 0);
    while (retval < 0) {
        attempts++;
        if (attempts <=
            Config::max_attempts) { //ponowne wysyłanie dopóki nie przekroczy liczby prób
            stringstream ss;
            ss << "Ponowne wysyłanie (próba " << attempts << ".)...";
            console_out(ss.str());
            sleep_ms(attempts * Config::wait_if_failed);
            retval = send(sockets.at(sindex), msg2, len, 0);
        } else {
            network_error("Nie udało się wysłać pakietu");
            delete[] msg2;
            return false;
        }
    }
    if (retval < len) {
        console_out("Pakiet niewysłany w całości, wysyłam resztę...");
        send_packet(sockets.at(sindex), msg2 + retval, len - retval);
        delete[] msg2;
        return true;
    }
    delete[] msg2;
    //console_out("Pomyślnie wysłano pakiet.");
    return true;
}

bool Network::read_packet(int sindex) {
    if (sindex < 0 || sindex >= (int) sockets.size()) return false;
    memset(recv_buffer, 0, Config::buffer_size);
    int retval = recv(sockets.at(sindex), recv_buffer, Config::buffer_size, 0);
    if (retval < 0) {
        network_error("Błąd odbierania pakietu");
        return false;
    }
    if (retval == 0) {
        fd_close(sindex);
        network_error("zamknięte połączenie");
        return false;
    }
    //dopisanie do odpowiedniego bufora odebranych danych
    for (int i = 0; i < retval; i++) {
        //jeśli wystąpił \n - przepisz zawartość do recv_packets i wyczyść bufor
        if (recv_buffer[i] == '\n' && recv_buffers.at(sindex)->size() > 0) {
            string s = "";
            for (unsigned int j = 0; j < recv_buffers.at(sindex)->size(); j++) {
                s += recv_buffers.at(sindex)->at(j);
            }
            recv_packets.at(sindex)->push_back(s);
            recv_buffers.at(sindex)->clear();
        } else {
            recv_buffers.at(sindex)->push_back(recv_buffer[i]);
        }
    }
    return true;
}

void Network::onConnectedToServer(int sindex) {
    recv_packets.at(sindex)->push_back("004");
}

void Network::updateSockets() {
    max_socket = 0;
    //wyczyszczenie zbioru monitorowanych socketów
    FD_ZERO(&read_sockets);
    if (server) {
        int server_socket = sockets.at(0);
        FD_SET(server_socket, &read_sockets); //monitorowanie socketu serwera
        max_socket = server_socket;
        //monitorowanie socketów podłączonych klientów
        for (unsigned int i = 1; i < sockets.size(); i++) {
            int client_socket = sockets[i];
            if (client_socket <= 0) {
                network_error("client_socket <= 0");
                return;
            }
            FD_SET(client_socket, &read_sockets);
            //maksymalny numer socketu
            if (client_socket > max_socket)
                max_socket = client_socket;
        }
    } else if (client) {
        int client_socket = sockets.at(0);
        FD_SET(client_socket, &read_sockets); //monitorowanie socketu połączenia z serwerem
        max_socket = client_socket;
    }
}
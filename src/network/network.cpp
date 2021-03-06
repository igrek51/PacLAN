#include "network.h"
#include "../config.h"
#include "../log/log.h"
#include "../utils.h"

#include <netdb.h>
#include <arpa/inet.h>
#include <boost/asio.hpp>

Network::Network(GameEngine* game_engine) : ContinuousThread(150) {
    this->game_engine = game_engine;
    server = false;
    client = false;
    temp_buffer = new char[Config::buffer_size];
    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 500;
    max_socket = 0;
    Log::info("Wątek interfejsu sieci zainicjalizowany.");
    init = true;
}

Network::~Network() {
    //zamykanie socketów
    if (server || client) {
        disconnect(0);
    }
    for (auto connection : connections) {
        delete connection;
    }
    connections.clear();
    delete[] temp_buffer;
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
    if (connections.size() > 0) {
        updateSocketsSet();
        //czekaj na aktywność jednego z socketów
        int activity = select(max_socket + 1, &read_sockets, NULL, NULL, &select_timeout);
        if (activity < 0) {
            error("select criticalError");
        }

        //przychodzące połączenie na serwerze
        if (server) {
            int server_socket = connections[0]->socket;
            if (FD_ISSET(server_socket, &read_sockets)) {
                onAccept();
            }
        }

        //operacja IO na pozostałych socketach
        for (unsigned i = 0; i < connections.size(); i++) {
            if (server && i == 0) {
                continue; //socket serwera tylko do nawiązywania nowych połączeń
            }
            int sd = connections[i]->socket;
            if (FD_ISSET(sd, &read_sockets)) {
                onReceive(i); //odebranie danych lub rozłączenie
            }
        }
    }

    sleep_ms(2); //zmniejszenie zużycia procesora
}

void Network::error(string e) {
    stringstream ss;
    ss << "Błąd sieci: " << e;
    Log::error(ss.str());
}

string Network::myIP() {
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

string Network::myName() {
    char* hostname = new char[128];
    gethostname(hostname, 128);
    string hname = hostname;
    delete[] hostname;
    return hname;
}

string Network::clientIP(int sindex) {
    if (sindex < 0 || sindex >= (int) connections.size())
        return "";
    sockaddr_in client_info;
    socklen_t client_info_size = sizeof(client_info);
    memset(&client_info, 0, sizeof(client_info));
    getpeername(connections[sindex]->socket, (sockaddr*) &client_info, &client_info_size);
    return inet_ntoa(client_info.sin_addr);
}

bool Network::openServerSocket() {
    if (client) {
        error("Otwarte jest już połączenie z hostem, zamknij je.");
        return false;
    }
    if (server) {
        Log::info("Restart serwera...");
        disconnect(0);
    } else {
        Log::info("Otwieram serwer...");
    }
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == 0) {
        error("openning server socket failed");
        return false;
    }

    int opt = 1;
    //wiele połączeń, wielokrotne użycie
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("failed setting server socket options");
        return false;
    }

    sockaddr_in serverInfo;
    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons((uint16_t) Config::port);

    if (bind(server_socket, (struct sockaddr*) &serverInfo, sizeof(serverInfo)) < 0) {
        error("błąd utworzenia serwera: bind");
        return false;
    }
    if (listen(server_socket, Config::max_clients) < 0) {
        error("błąd utworzenia serwera: błąd nasłuchiwania");
        return false;
    }

    connections.push_back(new NetworkConnection(server_socket));
    server = true;
    Log::info("Połączenie serwera otwarte.");
    return true;
}

bool Network::connectTo(string ip) {
    if (server) {
        error("Otwarty jest już serwer, zamknij połączenie serwera.");
        return false;
    }
    if (client) {
        Log::info("Restart połączenia z serwerem...");
        disconnect(0);
    } else {
        Log::info("Szukanie hosta: " + ip + " ...");
    }
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == 0) {
        error("creating client socket failed");
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
                error("Nie znaleziono hosta.");
                return false;
            }
            memcpy(&clientInfo.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
        } else {
            clientInfo.sin_addr.s_addr = inet_addr(ip.c_str());
        }
    }
    ip = inet_ntoa(clientInfo.sin_addr);
    clientInfo.sin_port = htons((uint16_t) Config::port);
    Log::info("Łączenie do: " + ip + " ...");
    if (connect(client_socket, (struct sockaddr*) &clientInfo, sizeof(struct sockaddr)) < 0) {
        error("Błąd połączenia do hosta");
        return false;
    }
    connections.push_back(new NetworkConnection(client_socket));
    int sindex = (int) (connections.size()) - 1;
    client = true;
    stringstream ss2;
    ss2 << "Połączono z: " << inet_ntoa(clientInfo.sin_addr);
    Log::info(ss2.str());
    onConnectedToServer(sindex);
    return true;
}


void Network::onClose(int sindex) {
    stringstream ss;
    ss << "Zamknięcie połączenia: socket index = " << sindex;
    Log::info(ss.str());
    if (client && sindex == 0) { //zamknięto serwer
        game_engine->cmd_on = true;
    }
    disconnect(sindex);
    if (server) { //zamknięto klienta
        ss_clear(ss);
        ss << "201 " << sindex;
        connections.at(0)->addReceivedPacket(ss.str());
    }
}

void Network::disconnect(int sindex) {
    if (sindex < 0 || sindex >= (int) connections.size()) return;
    if (client) { //zamknięcie klienta
        if (sindex == 0) {
            destroyConnection(sindex);
            client = false;
            Log::info("Zamknięto połączenie z serwerem.");
        }
    } else if (server) {
        if (sindex == 0) { //zamknięcie całego serwera
            //odłączenie klientów podłączonych do serwera
            while (connections.size() > 1) {
                destroyConnection(1);
            }
            destroyConnection(sindex);
            server = false;
            Log::info("Zamknięto serwer.");
        } else { //rozłączenie klienta od serwera
            destroyConnection(sindex);
        }
    }
}

void Network::destroyConnection(int sindex) {
    string ip;
    if (sindex >= 1) {
        ip = clientIP(sindex);
    }
    //zamykanie socketu
    if (!closeSocket(connections[sindex]->socket)) {
        error("błąd zamykania socketu");
    }
    delete connections[sindex];
    connections.erase(connections.begin() + sindex);
    if (sindex >= 1) {
        Log::info("Klient rozłączony: " + ip);
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
        Log::debug("Zamknięto socket.");
    }
    return true;
}

void Network::onAccept() {
    Log::info("Serwer: Żądanie nawiązania połączenia");
    if (server) {
        sockaddr_in clientInfo;
        socklen_t iAddrLen = sizeof(clientInfo);
        int new_client = accept(connections[0]->socket, (struct sockaddr*) &clientInfo, &iAddrLen);
        if (new_client < 0) {
            error("Błąd przy nawiązywaniu połączenia");
            return;
        }
        connections.push_back(new NetworkConnection(new_client));
        stringstream ss;
        ss << "Nawiązano połączenie z klientem: " << inet_ntoa(clientInfo.sin_addr);
        Log::info(ss.str());
    }
}

bool Network::sendData(int sindex, char* msg, int len) {
    if (sindex < 0 || sindex >= (int) connections.size()) {
        error("Numer socketu spoza zakresu");
        return false;
    }
    if (len == 0)
        return true;
    if (!client && !server) {
        error("Niepołączony");
        return false;
    }
    char* msg2 = new char[len];
    memcpy(msg2, msg, (size_t) len);

    int max_send_size = Config::buffer_size;
    if (max_send_size > 0) {
        if (len > max_send_size) { //podział na wiele pakietów
            bool wynik1 = sendData(connections[sindex]->socket, msg2, max_send_size);
            bool wynik2 = sendData(connections[sindex]->socket, msg2 + max_send_size,
                                   len - max_send_size);
            delete[] msg2;
            return wynik1 & wynik2;
        }
    }
    int attempts = 1;
    ssize_t retval = send(connections[sindex]->socket, msg2, (size_t) len, 0);
    while (retval < 0) {
        attempts++;
        if (attempts <=
            Config::max_attempts) { //ponowne wysyłanie dopóki nie przekroczy liczby prób
            stringstream ss;
            ss << "Ponowne wysyłanie (próba " << attempts << ".)...";
            Log::warn(ss.str());
            sleep_ms(attempts * Config::wait_if_failed);
            retval = send(connections[sindex]->socket, msg2, (size_t) len, 0);
        } else {
            error("Nie udało się wysłać pakietu");
            delete[] msg2;
            return false;
        }
    }
    if (retval < len) {
        Log::warn("Pakiet niewysłany w całości, wysyłam resztę...");
        sendData(connections[sindex]->socket, msg2 + retval, len - (int) retval);
        delete[] msg2;
        return true;
    }
    delete[] msg2;
    return true;
}

bool Network::onReceive(int sindex) {
    if (sindex < 0 || sindex >= (int) connections.size()) return false;
    memset(temp_buffer, 0, (size_t) Config::buffer_size);
    ssize_t retval = recv(connections[sindex]->socket, temp_buffer, (size_t) Config::buffer_size,
                          0);
    if (retval < 0) {
        error("Błąd odbierania pakietu");
        return false;
    }
    if (retval == 0) {
        onClose(sindex);
        error("zamknięte połączenie");
        return false;
    }
    //dopisanie do odpowiedniego bufora odebranych danych
    for (int i = 0; i < retval; i++) {
        //jeśli wystąpił \n - przepisz zawartość do recv_packets i wyczyść bufor
        if (temp_buffer[i] == '\n' && connections[sindex]->bufferSize() > 0) {
            string packet = connections[sindex]->bufferPop();
            connections[sindex]->addReceivedPacket(packet);
        } else {
            connections[sindex]->bufferAdd(temp_buffer[i]);
        }
    }
    return true;
}

void Network::onConnectedToServer(int sindex) {
    //TODO definicje numerów pakietów w enum class
    connections[sindex]->addReceivedPacket("004");
}

void Network::updateSocketsSet() {
    max_socket = 0;
    //wyczyszczenie zbioru monitorowanych socketów
    FD_ZERO(&read_sockets);
    if (server) {
        int server_socket = connections[0]->socket;
        FD_SET(server_socket, &read_sockets); //monitorowanie socketu serwera
        max_socket = server_socket;
        //monitorowanie socketów podłączonych klientów
        for (unsigned int i = 1; i < connections.size(); i++) {
            int client_socket = connections[i]->socket;
            if (client_socket <= 0) {
                error("client_socket <= 0");
                return;
            }
            FD_SET(client_socket, &read_sockets);
            //maksymalny numer socketu
            if (client_socket > max_socket)
                max_socket = client_socket;
        }
    } else if (client) {
        int client_socket = connections[0]->socket;
        FD_SET(client_socket, &read_sockets); //monitorowanie socketu połączenia z serwerem
        max_socket = client_socket;
    }
}
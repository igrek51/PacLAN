#include "network.h"
#include "config.h"
#include "log.h"
#include "app.h"

Network::Network() : ContinuousThread(150){
    App::network = this;
    error = false;
    server = false;
    client = false;
    recv_msg = new char [Config::buffer_size];
    wsa_start();
    log("Wątek interfejsu sieci zainicjalizowany.");
    init = true;
}

Network::~Network(){
    //zamykanie socketów
    if(server || client){
        disconnect_socket(0);
    }
    sockets.clear();
    delete[] recv_msg;
    recv_buffer.clear();
    recv_string.clear();
    tasks.clear();
    WSACleanup();
    log("Zamykanie wątku interfejsu sieci...");
}

void Network::runLoop(){
    //wykonanie zakolejkowanych zadań
    if(tasks.size()>0){
        task_exec(tasks.at(0));
        delete tasks.at(0);
        tasks.erase(tasks.begin());
    }
    //odbieranie i obsługa eventów
    if(sockets.size()>0){
        unsigned int event = WSAWaitForMultipleEvents(events.size(), &events[0], false, Config::network_refresh, false);
        if(event==WSA_WAIT_FAILED){
            network_error("WSA_WAIT_FAILED");
        }else if(event!=WSA_WAIT_TIMEOUT){
            WSANETWORKEVENTS NetworkEvents;
            if(WSAEnumNetworkEvents(sockets.at(event-WSA_WAIT_EVENT_0), events.at(event-WSA_WAIT_EVENT_0), &NetworkEvents)==SOCKET_ERROR){
                network_error("WSAEnumNetworkEvents: SOCKET_ERROR");
            }else{
                if(NetworkEvents.lNetworkEvents & FD_ACCEPT){ //żądanie nawiązania połączenia
                    fd_accept();
                }else if(NetworkEvents.lNetworkEvents & FD_CLOSE){ //rozłączono
                    fd_close(event-WSA_WAIT_EVENT_0);
                }else if(NetworkEvents.lNetworkEvents & FD_READ){ //odebranie bajtów
                    int sindex = event-WSA_WAIT_EVENT_0;
                    read_packet(sindex);
                }else if(NetworkEvents.lNetworkEvents & FD_CONNECT){ //pomyślnie połączono
                    recv_string.at(event-WSA_WAIT_EVENT_0)->push_back("004");
                }
            }
        }
    }
    Sleep(2); //zmniejszenie zużycia procesora
}

void Network::network_error(string e){
    error = true;
    stringstream ss;
    ss<<"[!] Błąd sieci ("<<WSAGetLastError()<<"): "<<e;
    console_out(ss.str());
}

void Network::console_out(string e){
    App::game_engine->cmd_output(e);
}

void Network::wsa_start(){
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
        network_error("WSAStartup");
        App::exit = true;
    }
}

string Network::get_myip(){
    char *hostname = new char[128];
    gethostname(hostname,128);
    HOSTENT *hp = gethostbyname(hostname);
    delete[] hostname;
    SOCKADDR_IN source;
    if(hp){
        memcpy(&(source.sin_addr),hp->h_addr,hp->h_length);
        return inet_ntoa(source.sin_addr);
    }
    return "";
}

string Network::get_myname(){
    char *hostname = new char[128];
    gethostname(hostname,128);
    string hname = hostname;
    delete[] hostname;
    return hname;
}

string Network::get_client_ip(int sindex){
    if(sindex<0 || sindex>=(int)sockets.size())
        return "";
    SOCKADDR_IN client_info;
    int client_info_size = sizeof(client_info);
    memset(&client_info, 0, sizeof(client_info));
    getpeername(sockets.at(sindex), (LPSOCKADDR)&client_info, &client_info_size);
    return inet_ntoa(client_info.sin_addr);
}

bool Network::open_server_socket(){
    if(client){
        network_error("Otwarte jest już połączenie z hostem, zamknij je.");
        return false;
    }
    if(server){
        console_out("Restartuje serwer...");
        disconnect_socket(0);
    }else{
        console_out("Otwieram serwer...");
    }
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket==(unsigned int)SOCKET_ERROR){
        network_error("open_server_socket: SOCKET_ERROR");
        return false;
    }
    SOCKADDR_IN serverInfo;
    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(Config::port);
    if(bind(server_socket, (LPSOCKADDR)&serverInfo, sizeof(serverInfo))==SOCKET_ERROR){
        network_error("open_server_socket: błąd utworzenia serwera");
        return false;
    }
    if(listen(server_socket,Config::max_clients)==SOCKET_ERROR){
        network_error("open_server_socket: błąd nasłuchiwania");
        return false;
    }
    WSAEVENT server_event = WSACreateEvent();
    if(WSAEventSelect(server_socket, server_event, FD_ACCEPT|FD_CLOSE)==SOCKET_ERROR){
        network_error("open_server_socket: błąd włączania sygnalizacji zdarzeń");
        WSACloseEvent(server_event);
        return false;
    }
    sockets.push_back(server_socket);
    events.push_back(server_event);
    recv_buffer.push_back(new vector<char>);
    recv_string.push_back(new vector<string>);
    server = true;
    console_out("Połączenie serwera otwarte.");
    return true;
}

bool Network::connect_socket(string ip){
    if(server){
        network_error("Otwarty jest już serwer, zamknij połączenie serwera.");
        return false;
    }
    if(client){
        console_out("Restart połączenia z serwerem...");
        disconnect_socket(0);
    }else{
        console_out("Szukanie hosta: "+ip+" ...");
    }
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_socket==INVALID_SOCKET){
        network_error("connect_socket: INVALID_SOCKET");
        return false;
    }
    SOCKADDR_IN clientInfo;
    memset(&clientInfo, 0, sizeof(clientInfo));
    clientInfo.sin_family = AF_INET;
    if(ip=="0"){
        clientInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        if(inet_addr(ip.c_str())==INADDR_NONE){
            LPHOSTENT hostEntry = gethostbyname(ip.c_str());
            if(!hostEntry){
                network_error("Nie znaleziono hosta.");
                return false;
            }
            clientInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
        }else{
            clientInfo.sin_addr.s_addr = inet_addr(ip.c_str());
        }
    }
    ip = inet_ntoa(clientInfo.sin_addr);
    clientInfo.sin_port = htons(Config::port);
    console_out("Łączenie do: "+ip+" ...");
    if(connect(client_socket,(LPSOCKADDR)&clientInfo,sizeof(struct sockaddr))==SOCKET_ERROR){
        network_error("Błąd połączenia do hosta");
        return false;
    }
    WSAEVENT client_event = WSACreateEvent();
    if(WSAEventSelect(client_socket, client_event, FD_READ|FD_CONNECT|FD_CLOSE)==SOCKET_ERROR){
        network_error("connect_socket: błąd włączania sygnalizacji zdarzeń");
        WSACloseEvent(client_event);
        return false;
    }
    sockets.push_back(client_socket);
    events.push_back(client_event);
    recv_buffer.push_back(new vector<char>);
    recv_string.push_back(new vector<string>);
    client = true;
    stringstream ss2;
    ss2<<"Połączono z: "<<inet_ntoa(clientInfo.sin_addr);
    console_out(ss2.str());
    return true;
}

void Network::remove_socket(int sindex){
    string ip;
    if(sindex>=1){
        ip = get_client_ip(sindex);
    }
    //zakończenie eventu
    WSACloseEvent(events.at(sindex));
    events.erase(events.begin()+sindex);
    //zamykanie socketu
    if(closesocket(sockets.at(sindex))==SOCKET_ERROR){
        network_error("closesocket: błąd zamykania socketu");
        return;
    }
    sockets.erase(sockets.begin()+sindex);
    //usunięcie bufora odbioru danych
    recv_buffer.at(sindex)->clear();
    recv_buffer.erase(recv_buffer.begin()+sindex);
    recv_string.at(sindex)->clear();
    recv_string.erase(recv_string.begin()+sindex);
    if(sindex>=1){
        console_out("Klient rozłączony: "+ip);
    }
}

void Network::disconnect_socket(int sindex){
    if(sindex<0 || sindex>=(int)sockets.size()) return;
    if(client){ //zamknięcie klienta
        if(sindex==0){
            remove_socket(sindex);
            client = false;
            console_out("Zamknięto połączenie z serwerem.");
        }
    }else if(server){
        if(sindex==0){ //zamknięcie całego serwera
            //odłączenie klientów podłączonych do serwera
            while(sockets.size()>1){
                remove_socket(1);
            }
            remove_socket(sindex);
            server = false;
            console_out("Zamknięto serwer.");
        }else{ //rozłączenie klienta od serwera
            remove_socket(sindex);
        }
    }
}

void Network::fd_accept(){
    console_out("Serwer: Żądanie nawiązania połączenia");
    if(server){
        SOCKADDR_IN clientInfo;
        int iAddrLen = sizeof(clientInfo);
        SOCKET new_client = accept(sockets.at(0), (struct sockaddr *) &clientInfo, &iAddrLen);
        if(new_client==(unsigned int)SOCKET_ERROR){
            network_error("Błąd przy nawiązywaniu połączenia");
            return;
        }
        WSAEVENT new_event = WSACreateEvent();
        if(WSAEventSelect(new_client, new_event, FD_READ|FD_CLOSE)==SOCKET_ERROR){
            network_error("fd_accept: błąd włączania sygnalizacji zdarzeń");
            WSACloseEvent(new_event);
            return;
        }
        sockets.push_back(new_client);
        events.push_back(new_event);
        recv_buffer.push_back(new vector<char>);
        recv_string.push_back(new vector<string>);
        stringstream ss;
        ss<<"Nawiązano połączenie z klientem: "<<inet_ntoa(clientInfo.sin_addr);
        console_out(ss.str());
    }
}

void Network::fd_close(int sindex){
    stringstream ss;
    ss<<"Zamknięcie połączenia: socket "<<sindex;
    console_out(ss.str());
    if(client && sindex==0){ //zamknięto serwer
        App::game_engine->cmd_on = true;
    }
    disconnect_socket(sindex);
    if(server){ //zamknięto klienta
        ss_clear(ss);
        ss<<"201 "<<sindex;
        recv_string.at(0)->push_back(ss.str());
    }
}

bool Network::send_packet(int sindex, char *msg, int len){
    if(sindex<0 || sindex>=(int)sockets.size()){
        network_error("Numer socketu spoza zakresu");
        return false;
    }
    if(len==0)
        return true;
    if(client==false && server==false){
        network_error("Niepołączony");
        return false;
    }
    char *msg2 = new char [len];
    memcpy(msg2,msg,len);
    //podział pakietów
    int max_send_size = Config::buffer_size;
    if(max_send_size>0){
        if(len>max_send_size){
            bool wynik1 = send_packet(sockets.at(sindex),msg2,max_send_size);
            bool wynik2 = send_packet(sockets.at(sindex),msg2+max_send_size,len-max_send_size);
            delete[] msg2;
            return wynik1&wynik2;
        }
    }
    int attempts = 1;
    int retval = send(sockets.at(sindex),msg2,len,0);
    while(retval==SOCKET_ERROR){
        attempts++;
        if(attempts <= Config::max_attempts){ //ponowne wysyłanie dopóki nie przekroczy liczby prób
            stringstream ss;
            ss<<"Ponowne wysyłanie (próba "<<attempts<<".)...";
            console_out(ss.str());
            Sleep(attempts * Config::wait_if_failed);
            retval = send(sockets.at(sindex),msg2,len,0);
        }else{
            network_error("Nie udało się wysłać pakietu");
            delete[] msg2;
            return false;
        }
    }
    if(retval<len){
        console_out("Pakiet niewysłany w całości, wysyłam resztę...");
        send_packet(sockets.at(sindex),msg2+retval,len-retval);
        delete[] msg2;
        return true;
    }
    delete[] msg2;
    //console_out("Pomyślnie wysłano pakiet.");
    return true;
}

bool Network::read_packet(int sindex){
    if(sindex<0 || sindex>=(int)sockets.size()) return false;
    memset(recv_msg, 0, Config::buffer_size);
    int retval = recv(sockets.at(sindex), recv_msg, Config::buffer_size, 0);
    if(retval==SOCKET_ERROR){
        network_error("Błąd odbierania pakietu");
        return false;
    }
    if(retval==0){
        network_error("Błąd odbierania - zamknięte połączenie");
        return false;
    }
    //dopisanie do odpowiedniego bufora odebranych danych
    for(int i=0; i<retval; i++){
        //jeśli wystąpił \n - przepisz zawartość do recv_string i wyczyść bufor
        if(recv_msg[i]=='\n' && recv_buffer.at(sindex)->size()>0){
            string s = "";
            for(unsigned int j=0; j<recv_buffer.at(sindex)->size(); j++){
                s+=recv_buffer.at(sindex)->at(j);
            }
            recv_string.at(sindex)->push_back(s);
            recv_buffer.at(sindex)->clear();
        }else{
            recv_buffer.at(sindex)->push_back(recv_msg[i]);
        }
    }
    return true;
}

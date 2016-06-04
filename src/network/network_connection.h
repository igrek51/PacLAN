/**
 * \file network_connection.h
 * \see network_connection.cpp
 */
#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <string>
#include <deque>

using namespace std;

class NetworkConnection {
private:
    /// bufor danych dla każdego połączenia - zawiera całe, kompletne pakiety
    deque<string> recv_packet;

    /// bufor danych dla każdego połączenia - zawiera ciągi wszystkich odebranych znaków, bufory odebranych danych od serwera (indeks 0) lub od klientów serwera (indeksy większe 0)
    deque<char> recv_buffer;
public:

    int socket;

    NetworkConnection(int socket);

    ~NetworkConnection();

    void addReceivedPacket(string packet);

    bool isReady();

    string popPacket();

    string bufferPop();

    void bufferAdd(char c);

    unsigned int bufferSize();
};


#endif

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
    /// numer socketu
    int socket;

    /**
     * konstruktor
     * @param socket numer socketu
     */
    NetworkConnection(int socket);

    ~NetworkConnection();

    /**
     * dodaje komunikat do kolejki pakietów
     * @param packet tekstowy komunikat
     */
    void addReceivedPacket(string packet);

    /**
     * @return czy pakiet jest gotowy do odebrania
     */
    bool isReady();

    /**
     * zdejmuje pierwszy pakiet z początku kolejki i zwraca go
     * @return pakiet z początku kolejki
     */
    string popPacket();

    /**
     * zdejmuje dane z bufora i zwraca je
     * @return dane z bufora
     */
    string bufferPop();

    /**
     * dodaje znak na koniec bufora danych
     */
    void bufferAdd(char c);

    /**
     * @return rozmiar aktualnego bufora danych
     */
    unsigned int bufferSize();
};


#endif

/**
 * \file network.h
 * Obsługa połączeń sieciowych
 * \see network.cpp
 * \see network_task.h
 */
#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include "thread.h"
#include "network_task.h"

using namespace std;

/**
 * \brief Silnik obsługi komunikacji między wieloma komputerami
 */
class Network : public Thread {
public:
    /// uruchomienie wątku sieci
    Network();
    /// zakończenie wątku sieci
    ~Network();
private:
    /// główna funkcja wątku
    void run();

    //  SIEĆ
public:
    /**
     * zwraca adres ip komputera jako tekst
     * @return adres ip
     */
    string get_myip();
    /**
     * zwraca nazwę komputera w sieci
     * @return nazwa komputera
     */
    string get_myname();
    /**
     * zwraca adres ip wybranego klienta podłączonego do hosta
     * @param sindex indeks klienta na liście socketów
     * @return adres ip klienta w postaci tekstu
     */
    string get_client_ip(int sindex);
private:
    /// klasa do inicjalizacji sieci
    WSADATA wsaData;
    /// inicjalizacja sieci
    void wsa_start();

    //  SOCKETY
public:
    /// lista Socketów - otwartych połączeń z innymi komputerami, 0 - socket klienta lub socket serwera, większe od 0 - sockety klientów serwera
    vector<SOCKET> sockets;
    /// zmienna określająca, czy serwer jest otwarty (true - otwarty)
    bool server;
    /// zmienna określająca, czy połączenie klienta jest aktywne (true - aktywne)
    bool client;

    //  EVENTY
private:
    /// wektor do obsługi zdarzeń socketów, liczba eventów = liczba socketów, 0 - event servera lub clienta, większe od 0 - klienci serwera
    vector<WSAEVENT> events;

    //  BUFORY DANYCH
public:
    /// bufor danych dla każdego połączenia - zawiera całe, kompletne pakiety
    vector< vector<string>* > recv_string;
private:
    /// bufor danych dla każdego połączenia - zawiera ciągi wszystkich odebranych znaków, bufory odebranych danych od serwera (indeks 0) lub od klientów serwera (indeksy większe 0)
    vector< vector<char>* > recv_buffer;
    ///tymczasowy bufor aktualnej wiadomości
    char *recv_msg;
    ///podziel bufory recv_buffer na recv_string
    void split_recv();

    //  ZADANIA
public:
    /**
     * dodaje zadanie wysłania wiadomości do serwera
     * @param msg tablica znaków do wysłania
     * @param len ilość przesyłanych znaków z tablicy
     */
    void addtask_send_to_server(char *msg, int len);
    /**
     * dodaje zadanie wysłania wiadomości do klienta
     * @param client_nr numer klienta
     * @param msg tablica znaków do wysłania
     * @param len ilość przesyłanych znaków z tablicy
     */
    void addtask_send_to_client(int client_nr, char *msg, int len);
    /// dodaje zadanie otworzenia połączenia serwera
    void addtask_open_server();
    /**
     * dodaje zadania połączenia klienta do serwera
     * @param ip adres ip serwera
     */
    void addtask_connect_client(string ip);
    /// dodaje zadanie zamknięcia serwera
    void addtask_close_server();
    ///dodaje zadanie zamknięcia połączenia z serwerem
    void addtask_close_client();
private:
    ///// kolejka zadań do wykonania
    vector<NetworkTask*> tasks;
    /**
     * wykonanie zadania
     * @param task wskaźnik na zadanie sieciowe
     */
    void task_exec(NetworkTask *task);

    //  KONTROLA BŁĘDÓW
    /// zmienna, która określa, czy wystąpił błąd
    bool error;
    /**
     * wyświetlenie szczegółowej informacji o błędzie
     * @param e wiadomość do wyświetlenia
     */
    void network_error(string e);
    /**
     * wyświetlenie informacji w konsoli
     * @param e treść informacji
     */
    void console_out(string e);

    //  POŁĄCZENIE
    /**
     * otworzenie połączenia serwera
     * @return true - jeśli udało się połączyć
     */
    bool open_server_socket();
    /**
     * połączenie do serwera o danym ip
     * @param ip adres ip serwera
     * @return true - jeśli udało się połączyć
     */
    bool connect_socket(string ip);
    /**
     * usunięcie wskazanego socketu, eventu i bufora danych
     * @param sindex numer połączenia
     */
    void remove_socket(int sindex);
    /**
     * rozłączenie połączenia
     * @param sindex numer połączenia
     */
    void disconnect_socket(int sindex);
    /**
     * zamknięcie połączenia - wiadomość od serwera lub klienta
     * @param sindex numer połączenia
     */
    void fd_close(int sindex);
    /// żądanie nawiązania połączenia - wiadomość od klienta
    void fd_accept();

    //  TRANSMISJA DANYCH
    /**
     * wyślij pakiet do odbiorcy
     * @param sindex numer połączenia - odbiorcy pakietu
     * @param tablica znaków do wysłania
     * @param len ilość przesyłanych znaków z tablicy
     * @return true - jeśli wszystko przebiegło pomyślnie
     */
    bool send_packet(int sindex, char *msg, int len);
    /**
     * odbierz pakiet od nadawcy
     * @param sindex numer połączenia nadawcy
     * @return true - jeśli wszystko przebiegło pomyślnie
     */
    bool read_packet(int sindex);
};

#endif

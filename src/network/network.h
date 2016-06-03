#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include "../threads/continuous_thread.h"
#include "network_task.h"
#include "network_connection.h"
#include "../logic/game_engine.h"

using namespace std;

/**
 * \brief Silnik obsługi komunikacji między wieloma komputerami
 */
class Network : public ContinuousThread {
public:
    /// uruchomienie wątku sieci
    Network(GameEngine* game_engine);
    /// zakończenie wątku sieci
    ~Network();
protected:
    /// główna funkcja powtarzana w pętli
    void runLoop();

public:
    /**
     * zwraca adres ip komputera jako tekst
     * @return adres ip
     */
    string myIP();
    /**
     * zwraca nazwę komputera w sieci
     * @return nazwa komputera
     */
    string myName();
    /**
     * zwraca adres ip wybranego klienta podłączonego do hosta
     * @param sindex indeks klienta na liście socketów
     * @return adres ip klienta w postaci tekstu
     */
    string clientIP(int sindex);
    //  SOCKETY

    /// zmienna określająca, czy serwer jest otwarty (true - otwarty)
    bool server;
    /// zmienna określająca, czy połączenie klienta jest aktywne (true - aktywne)
    bool client;
    /// lista otwartych połączeń z innymi komputerami, indeks 0 - klient lub socket serwera, większe od 0 - sockety klientów serwera
    vector<NetworkConnection*> connections;

    //  ZADANIA
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

    ///tymczasowy bufor aktualnej wiadomości
    char *temp_buffer;

    timeval select_timeout;
    fd_set read_sockets;
    int max_socket;

    GameEngine* game_engine;

    ///// kolejka zadań do wykonania
    vector<NetworkTask*> tasks;
    /**
     * wykonanie zadania
     * @param task wskaźnik na zadanie sieciowe
     */
    void task_exec(NetworkTask *task);

    //  KONTROLA BŁĘDÓW
    /**
     * wyświetlenie szczegółowej informacji o błędzie
     * @param e wiadomość do wyświetlenia
     */
    void error(string e);

    //  POŁĄCZENIE
    /**
     * otworzenie połączenia serwera
     * @return true - jeśli udało się połączyć
     */
    bool openServerSocket();
    /**
     * połączenie do serwera o danym ip
     * @param ip adres ip serwera
     * @return true - jeśli udało się połączyć
     */
    bool connectTo(string ip);
    /**
     * usunięcie wskazanego socketu, eventu i bufora danych
     * @param sindex numer połączenia
     */
    void destroyConnection(int sindex);
    /**
     * rozłączenie połączenia
     * @param sindex numer połączenia
     */
    void disconnect(int sindex);
    /**
     * zamknięcie połączenia - wiadomość od serwera lub klienta
     * @param sindex numer połączenia
     */
    void onClose(int sindex);
    /// żądanie nawiązania połączenia - wiadomość od klienta
    void onAccept();

    void onConnectedToServer(int sindex);

    //  TRANSMISJA DANYCH
    /**
     * wyślij pakiet do odbiorcy
     * @param sindex numer połączenia - odbiorcy pakietu
     * @param tablica znaków do wysłania
     * @param len ilość przesyłanych znaków z tablicy
     * @return true - jeśli wszystko przebiegło pomyślnie
     */
    bool sendData(int sindex, char* msg, int len);
    /**
     * odbierz pakiet od nadawcy
     * @param sindex numer połączenia nadawcy
     * @return true - jeśli wszystko przebiegło pomyślnie
     */
    bool onReceive(int sindex);

    bool closeSocket(int socket);

    void updateSocketsSet();
};

#endif

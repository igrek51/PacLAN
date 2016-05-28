/**
 * \file network_task.h
 * Zadania do wykonania w obrębie obsługi sieci
 * \see network_task.cpp
 * \see network.h
 */
#ifndef NETWORK_TASK_H
#define NETWORK_TASK_H

#define TASK_OPEN_SERVER 1
#define TASK_CLOSE_SERVER 2
#define TASK_CONNECT 3 //połącz klienta do serwera
#define TASK_CLOSE 4 //rozłącz klienta przez klienta
#define TASK_KICK_CLIENT 5 //rozłączenie klienta przez serwer
#define TASK_SEND_TO_SERVER 6 //wyślij pakiet do serwera
#define TASK_SEND_TO_CLIENT 7 //wyślij pakiet do jednego z klientów
#define TASK_RECEIVE 8

#include <iostream>
#include <vector>

using namespace std;

/**
 * \brief Sieciowe zadanie do wykonania należące do kolejki zadań
 */
class NetworkTask {
public:
    /**
     * Utworzenie nowego zadania sieciowego
     * \param task numer zadania do wykonania
     */
    NetworkTask(int task);
    ~NetworkTask();
    /// numer zadania do wykonania
    int task;
    /// możliwy do wykorzystania pierwszy całkowity parametr zadania
    int param_i;
    /// możliwy do wykorzystania drugi całkowity parametr zadania
    int param_i2;
    /// możliwa do wykorzystania przez zadanie tablica znaków
    char *param_c;
    /// możliwa do wykorzystania przez zadanie ciąg znaków klasy string
    string param_s;
};

#endif

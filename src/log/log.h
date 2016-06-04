/**
 * \file log.h
 * \see log.cpp
 */
#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <list>
#include "log_level.h"

using namespace std;

/**
 * \brief Zgłaszanie komunikatów na różnych poziomach
 */
class Log {
public:
    /**
     * zgłoszenie komunikatu błędu
     * @param s komunikat
     */
    static void error(string s);

    /**
     * zgłoszenie komunikatu ostrzeżenia
     * @param s komunikat
     */
    static void warn(string s);

    /**
     * zgłoszenie komunikatu informacji
     * @param s komunikat
     */
    static void info(string s);

    /**
     * zgłoszenie komunikatu na poziomie debug
     * @param s komunikat
     */
    static void debug(string s);

    /**
     * zgłoszenie i wyświetlenie komunikatu o błędzie krytycznym
     * @param s treść komunikatu
     */
    static void criticalError(string s);

    /**
     * @return czy wystąpił błąd krytyczny
     */
    static bool wasCriticalError();

    /**
     * @return lista ostatnich komunikatów wyświetlanych użytkownikowi w konsoli
     */
    static list<string>* getEchoes();

private:
    /// instancja singletonowej klasy
    static Log* instance;

    /**
     * @return jedyna instancja klasy
     */
    static Log* getInstance();

    ///liczba krytycznych błędów
    int criticalErrorsCount;

    /// lista ostatnich komunikatów wyświetlanych w konsoli
    list<string> echoes;

    //konstruktor
    Log();

    //destruktor
    ~Log();

    /**
     * zapisuje komunikat do pliku (w zależności od poziomu)
     * @param s komunikat
     * @param level poziom ważności komunikatu
     */
    void log(string s, LogLevel level);

    /**
     * wyświetla komunikat w konsoli (w zależności od poziomu)
     * @param s komunikat
     * @param level poziom ważności komunikatu
     */
    void echo(string s, LogLevel level);

    /// wyczyszczenie zawartości pliku dziennika
    void clearLog();
};

#endif

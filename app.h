/**
 * \file app.h
 * Moduł głównej aplikacji
 * \see app.cpp
 */
#ifndef APP_H
#define APP_H

#include "game_engine.h"
#include "graphics.h"
#include "network.h"
#include "timer.h"

/**
 * \brief Klasa głównej aplikacji zawierająca jej najważniejsze moduły
 */
class App{
public:
    /// utworzenie nowej instancji aplikacji
    App();
    /// zakończenie uruchomionej aplikacji
    ~App();
    /// wskaźnik na obiekt silnika gry
    static GameEngine *game_engine;
    /// wskaźnik na obiekt silnika grafiki
    static Graphics *graphics;
    /// wskaźnik na wątek modułu obsługi sieci
    static Network *network;
    /// wskaźnik na wątek czasomierza
    static Timer *timer;
    /// zmienna określająca, czy zakończyć aplikację (true - sygnał zakończenia)
    static bool exit;
    /// aktualna liczba cykli do wykonania (zwiększana przez timer, zmniejszana przez pętle logiczne silnika gry)
    static volatile int logic_cycles;
};

#endif

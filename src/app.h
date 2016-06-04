/**
 * \file app.h
 * \see app.cpp
 */
#ifndef APP_H
#define APP_H

class App;

#include "logic/game_engine.h"
#include "graphics/graphics.h"
#include "network/network.h"
#include "threads/timer.h"
#include "language.h"

/**
 * \brief Aplikacja zawierająca główne moduły
 */
class App {
private:
    /// sygnał zakończenia aplikacji
    bool _exit;
public:
    /// konstruktor
    App();

    /// destruktor
    ~App();

    /// silnik gry (logika)
    GameEngine* game_engine;
    /// silnik grafiki
    Graphics* graphics;
    /// wątek modułu obsługi sieci
    Network* network;
    /// wątek odmierzania czasu i kontroli prędkości gry
    Timer* timer;
    /// aktualne dane językowe
    Language* lang;
    /// aktualna liczba cykli do wykonania (zwiększana przez timer, zmniejszana przez pętle logiczne silnika gry)
    volatile int logic_cycles;

    // czy należy zamknąć aplikację
    bool exiting();

    /// zgłoszenie sygnału zakończenia aplikacji
    void exit();
};

#endif

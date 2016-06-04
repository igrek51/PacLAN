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
 * \brief Główna aplikacja zawierająca najważniejsze moduły
 */
class App {
private:
    bool _exit;
public:
    /// konstruktor
    App();
    /// destruktor
    ~App();
    /// wskaźnik na silnik gry
    GameEngine* game_engine;
    /// wskaźnik na silnik grafiki
    Graphics* graphics;
    /// wskaźnik na wątek modułu obsługi sieci
    Network* network;
    /// wskaźnik na wątek odmierzania czasu i kontroli prędkości gry
    Timer* timer;
    /// aktualne dane językowe
    Language* lang;

    /// aktualna liczba cykli do wykonania (zwiększana przez timer, zmniejszana przez pętle logiczne silnika gry)
    volatile int logic_cycles;


    bool exiting();

    /// sygnał zakończenia aplikacji
    void exit();
};

#endif

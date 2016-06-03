#ifndef APP_H
#define APP_H

#include "logic/game_engine.h"
#include "graphics/graphics.h"
#include "network/network.h"
#include "threads/timer.h"
#include "language.h"

/**
 * \brief Klasa głównej aplikacji zawierająca jej najważniejsze moduły
 */
class App {
public:
    /// utworzenie nowej instancji aplikacji
    App();

    /// zakończenie uruchomionej aplikacji
    ~App();

    /// wskaźnik na obiekt silnika gry
    GameEngine* game_engine;
    /// wskaźnik na obiekt silnika grafiki
    Graphics* graphics;
    /// wskaźnik na wątek modułu obsługi sieci
    Network* network;
    /// wskaźnik na wątek czasomierza
    Timer* timer;
    /// aktualne dane językowe
    Language* lang;
    /// zmienna określająca, czy zakończyć aplikację (true - sygnał zakończenia)
    static bool exit;
    /// aktualna liczba cykli do wykonania (zwiększana przez timer, zmniejszana przez pętle logiczne silnika gry)
    static volatile int logic_cycles;
};

#endif

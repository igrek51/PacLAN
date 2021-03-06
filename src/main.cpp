/**
* \mainpage
* PacLAN - gra PacMan umożliwiająca grę w sieci dla wielu graczy
* \author Ireneusz Szulc
*/
#include "app.h"
#include "config.h"
#include "utils.h"
#include "log/log.h"

//TODO pakiet synchronizująco - weryfikujący
//TODO dźwięki, muzyka z Pacmana

//TODO smart_ptr (unique, shared) ...

int main(int argc, char** argv) {
    App* app = new App();
    //inicjalizacja grafiki
    app->graphics = new Graphics();
    if (Log::wasCriticalError()) return 1;
    //logika gry
    app->game_engine = new GameEngine(app, app->graphics);
    if (Log::wasCriticalError()) return 1;
    app->graphics->setGameEngine(app->game_engine);
    //interfejs sieciowy
    app->network = new Network(app->game_engine);
    if (Log::wasCriticalError()) return 1;
    app->game_engine->setNetwork(app->network);
    //timer utrzymujący stałą prędkość gry
    app->timer = new Timer(Config::logic_timer_ms, &app->logic_cycles);

    while (!app->exiting()) {
        while (app->logic_cycles > 0 && !app->exiting()) { //jeśli są do wykonania cykle (zaległe)
            app->game_engine->logic(app->logic_cycles); //wykonuj obliczenia gry
        }
        app->graphics->draw(); //jeśli zostanie czasu - repaint grafiki
        sleep_ms(1); //zmniejszenie zużycia procesora
    }

    delete app;
    return 0;
}



/**
* \mainpage
* PacLAN - gra PacMan umożliwiająca rozgrywkę przez sieć dla wielu graczy
* \author Ireneusz Szulc
*/
#include <windows.h>
#include "app.h"
#include "config.h"

int main(int argc, char **argv){
    App *app = new App();
    app->graphics = new Graphics(); //inicjalizacja grafiki
    if(app->exit)
        return 0;
    app->game_engine = new GameEngine(); //silnik gry
    if(app->exit)
        return 0;
    app->network = new Network(); //interfejs sieciowy
    if(app->exit)
        return 0;
    app->timer = new Timer(Config::logic_timer_ms,&app->logic_cycles); //timer utrzymujący stałą prędkość gry
    while(!app->exit){
        while(app->logic_cycles>0 && !App::exit){ //jeśli są do wykonania cykle (zaległe)
            app->game_engine->logic(app->logic_cycles); //wykonuj obliczenia gry
        }
        app->graphics->draw(); //jeśli zostanie czasu - repaint grafiki
        Sleep(1); //zmniejszenie zużycia procesora
    }
    delete app;
    return 0;
}



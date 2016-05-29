#include "app.h"
#include "config.h"
#include "system.h"

//TODO przeniesienie plików źródłowych do folderów: threads, network, graphics, players, animations, ...

//TODO pakiet synchronizująco - weryfikujący
//TODO dźwięki, muzyka z Pacmana
//TODO dodanie tłumaczeń do wiersza poleceń

int main(int argc, char **argv){
    App *app = new App();
    app->graphics = new Graphics(); //inicjalizacja grafiki
    if(app->exit)
        return 0;
    app->game_engine = new GameEngine(); //logika gry
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
        sleep_ms(1); //zmniejszenie zużycia procesora
    }
    delete app;
    return 0;
}



#include "app.h"
#include "config.h"
#include "system.h"

//TODO pakiet synchronizująco - weryfikujący
//TODO dźwięki, muzyka z Pacmana

//TODO C++ 11: pętle foreach, auto, enum class, smart_ptr (unique, shared) ...
//TODO STL: przechodzenie kontenerów w pętli iteratorami, set, deque, list, ...

//TODO komentarze doxygena - porządek: wyjebać lub uzupełnić

//TODO optimize imports w każdym pliku - skrócenie czasu kompilacji: ograniczenie boosta i sdla

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



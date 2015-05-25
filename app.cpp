#include "app.h"
#include "log.h"

Graphics* App::graphics = NULL;
GameEngine* App::game_engine = NULL;
Network* App::network = NULL;
Timer* App::timer = NULL;
bool App::exit = false;
volatile int App::logic_cycles = 1;

App::App(){
    graphics = NULL;
    game_engine = NULL;
    network = NULL;
    timer = NULL;
    exit = false;
    logic_cycles = 1;
    srand(time(0));
    log_clear();
    log("Hello World!");
}

App::~App(){
    log("Zamykanie aplikacji...");
    exit = true;
    if(timer!=NULL)
        delete timer;
    if(graphics!=NULL)
        delete graphics;
    if(game_engine!=NULL)
        delete game_engine;
    if(network!=NULL)
        delete network;
    log("Goodbye World...");
}

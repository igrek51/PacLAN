#include "app.h"
#include "log/log.h"

bool App::exit = false;
volatile int App::logic_cycles = 1;

//TODO coś innego niż singleton: listenery

App::App(){
    graphics = nullptr;
    game_engine = nullptr;
    network = nullptr;
    timer = nullptr;
    lang = nullptr;
    exit = false;
    logic_cycles = 1;
    srand((unsigned int) time(0));
    Log::info("Inicjalizacja aplikacji...");
}

App::~App(){
    Log::info("Zamykanie aplikacji...");
    exit = true;
    if(timer!=nullptr)
        delete timer;
    if(graphics!=nullptr)
        delete graphics;
    if(game_engine!=nullptr)
        delete game_engine;
    if(network!=nullptr)
        delete network;
    if(lang!=nullptr)
        delete lang;
    Log::info("Aplikacja zamknięta.");
}

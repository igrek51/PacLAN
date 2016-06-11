#include "app.h"
#include "log/log.h"

App::App() {
    graphics = nullptr;
    game_engine = nullptr;
    network = nullptr;
    timer = nullptr;
    lang = nullptr;
    _exit = false;
    logic_cycles = 1;
    srand((unsigned int) time(0));
    Log::info("Inicjalizacja aplikacji...");
}

App::~App() {
    Log::info("Zamykanie aplikacji...");
    _exit = true;
    if (timer != nullptr)
        delete timer;
    if (graphics != nullptr)
        delete graphics;
    if (game_engine != nullptr)
        delete game_engine;
    if (network != nullptr)
        delete network;
    if (lang != nullptr)
        delete lang;
    Log::info("Aplikacja zamknięta.");
}

bool App::exiting() {
    return _exit || Log::wasCriticalError();
}

void App::exit() {
    _exit = true;
}
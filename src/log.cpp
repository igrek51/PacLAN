#include <fstream>
#include <SDL2/SDL.h>
#include "log.h"
#include "config.h"
#include "utils.h"

//TODO network error, sdl error, critical error
//TODO przekazywanie błędów wyjątkami

int Log::log_level = DEBUG;
int Log::echo_level = WARN;

int Log::errors_count = 0;

void Log::error(string s) {
    errors_count++;
    string s2 ="[ERROR] " + s;
    echo(s2, ERROR);
    log(s2, ERROR);
}

void Log::warn(string s) {
    string s2 ="[warn] " + s;
    echo(s2, WARN);
    log(s2, WARN);
}

void Log::info(string s) {
    echo(s, INFO);
    log(s, INFO);
}

void Log::debug(string s) {
    string s2 ="[debug] " + s;
    echo(s2, DEBUG);
    log(s2, DEBUG);
}


bool Log::isError() {
    return errors_count > 0;
}


void Log::log(string s, int level) {
    if (level > log_level) return;
    fstream plik;
    plik.open(Config::log_filename.c_str(), fstream::out | fstream::app);
    if (!plik.good()) {
        plik.close();
        return;
    }
    plik << get_time() << " - " << s << endl;
    plik.close();
}

void Log::echo(string s, int level) {
    if (level > echo_level) return;
    if (level == ERROR) {
        cerr << s << endl;
    } else {
        cout << s << endl;
    }
}


void Log::logClear() {
    fstream plik;
    plik.open(Config::log_filename.c_str(), fstream::out | fstream::trunc);
    plik.close();
}

void Log::criticalError(string s) {
    error(s);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd", s.c_str(), NULL);
}

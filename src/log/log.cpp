#include <fstream>
#include <SDL2/SDL.h>
#include "log.h"
#include "../config.h"
#include "../utils.h"

//TODO przekazywanie błędów wyjątkami

Log* Log::instance = nullptr;

Log::Log() {
    criticalErrorsCount = 0;
    clearLog();
}

Log::~Log() {
    echoes.clear();
}

Log* Log::getInstance() {
    if (instance == nullptr) {
        instance = new Log();
    }
    return instance;
}


void Log::error(string s) {
    Log* inst = getInstance();
    string s2 = "[ERROR] " + s;
    inst->echo(s2, LogLevel::ERROR);
    inst->log(s2, LogLevel::ERROR);
}

void Log::warn(string s) {
    Log* inst = getInstance();
    string s2 = "[warn] " + s;
    inst->echo(s2, LogLevel::WARN);
    inst->log(s2, LogLevel::WARN);
}

void Log::info(string s) {
    Log* inst = getInstance();
    inst->echo(s, LogLevel::INFO);
    inst->log(s, LogLevel::INFO);
}

void Log::debug(string s) {
    Log* inst = getInstance();
    string s2 = "[debug] " + s;
    inst->echo(s2, LogLevel::DEBUG);
    inst->log(s2, LogLevel::DEBUG);
}


void Log::log(string s, LogLevel level) {
    if (level > Config::file_log_level) return;
    fstream plik;
    plik.open(Config::log_filename.c_str(), fstream::out | fstream::app);
    if (!plik.good()) {
        plik.close();
        return;
    }
    plik << get_time() << " - " << s << endl;
    plik.close();
}

void Log::echo(string s, LogLevel level) {
    if (level > Config::echo_log_level) return;
    echoes.push_back(s);
    while (echoes.size() > Config::echoes_max) {
        echoes.pop_front();
    }
    if (level == LogLevel::ERROR) {
        cerr << s << endl;
    } else {
        cout << s << endl;
    }
}


void Log::clearLog() {
    fstream plik;
    plik.open(Config::log_filename.c_str(), fstream::out | fstream::trunc);
    plik.close();
}

void Log::criticalError(string s) {
    error(s);
    Log* inst = getInstance();
    inst->criticalErrorsCount++;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd", s.c_str(), NULL);
}

bool Log::wasCriticalError() {
    Log* inst = getInstance();
    return inst->criticalErrorsCount > 0;
}

list<string>* Log::getEchoes() {
    Log* inst = getInstance();
    return &(inst->echoes);
}
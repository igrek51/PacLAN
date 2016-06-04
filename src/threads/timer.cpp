#include <sstream>
#include "timer.h"
#include "../log/log.h"
#include "../config.h"
#include "../utils.h"

Timer::Timer(int interval_ms, volatile int* cycles) : ContinuousThread(150) {
    this->interval_ms = interval_ms; //konwersja ms na cykle
    this->cycles = cycles;
    next_time = currentMillis() + interval_ms;
    stringstream ss;
    ss << "Timer cykli logicznych zainicjalizowany z okresem " << interval_ms << " ms";
    Log::info(ss.str());
    init = true;
}

Timer::~Timer() {
    Log::info("Zamykanie wątku timera...");
}

void Timer::runLoop() {
    if (currentMillis() >= next_time) {
        next_time += interval_ms;
        (*cycles)++;
    }
    sleep_ms(1);
}

int Timer::cycles_to_s(int c) {
    return c * Config::logic_timer_ms / 1000 + 1; //liczba pozostałych sekund (+1)
}

int Timer::s_to_cycles(int s) {
    return s * 1000 / Config::logic_timer_ms;
}

#include <sstream>
#include "timer.h"
#include "log.h"
#include "config.h"
#include "system.h"

Timer::Timer(int interval_ms, volatile int *cycles) : ContinuousThread(150) {
    this->interval_ticks = interval_ms*CLOCKS_PER_SEC/1000; //konwersja ms na cykle
    this->cycles = cycles;
    last_time = clock();
    stringstream ss;
    ss<<"Timer cykli logicznych zainicjalizowany ("<<interval_ms<<" ms)";
    log(ss.str());
    init = true;
}

Timer::~Timer(){
    log("Zamykanie wątku timera...");
}

void Timer::runLoop(){
    if(clock() >= last_time + interval_ticks){
        last_time += interval_ticks;
        (*cycles)++;
    }
    sleep_ms(2);
}

int Timer::cycles_to_s(int c){
    return c * Config::logic_timer_ms/1000 + 1; //liczba pozostałych sekund (+1)
}

int Timer::s_to_cycles(int s){
    return s * 1000/Config::logic_timer_ms;
}
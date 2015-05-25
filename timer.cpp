#include <sstream>
#include "timer.h"
#include "log.h"
#include "config.h"

Timer::Timer(int interval_ms, volatile int *cycles){
    this->interval_ticks = interval_ms*CLOCKS_PER_SEC/1000; //konwersja ms na cykle
    this->cycles = cycles;
    last_time = clock();
    stringstream ss;
    ss<<"Timer cykli logicznych zainicjalizowany ("<<interval_ms<<" ms)";
    log(ss.str());
}

Timer::~Timer(){
    log("Wątek timera zakończony.");
}

void Timer::run(){
    while(true){
        if(clock() >= last_time + interval_ticks){
            last_time += interval_ticks;
            (*cycles)++;
        }
        Sleep(2);
    }
}

int Timer::cycles_to_s(int c){
    return c * Config::logic_timer_ms/1000 + 1; //liczba pozostałych sekund (+1)
}

int Timer::s_to_cycles(int s){
    return s * 1000/Config::logic_timer_ms;
}

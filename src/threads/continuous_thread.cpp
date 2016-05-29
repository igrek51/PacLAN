#include <boost/thread.hpp>
#include "continuous_thread.h"
#include "../log.h"
#include "../system.h"

ContinuousThread::ContinuousThread(int wait_for_close) {
    this->wait_for_close = wait_for_close;
    closed = false;
}

ContinuousThread::~ContinuousThread() {
    close_signal = true;
    Log::info("Oczekiwanie na zakończenie wątku...");
    for (int i = 0; i < wait_for_close && !closed; i++) {
        sleep_ms(1);
    }
    if (!closed) {
        Log::info("Przekroczono czas oczekiwania - siłowe zamykanie wątku.");
        boost_thread->interrupt();
        delete boost_thread;
        boost_thread = nullptr;
    }
    Log::info("Wątek zapętlonej procedury zakończony.");
}

void ContinuousThread::run() {
    while (!close_signal) {
        runLoop();
    }
    closed = true;
}
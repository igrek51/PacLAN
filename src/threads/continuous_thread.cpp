#include "continuous_thread.h"
#include "../log/log.h"
#include "../utils.h"
#include <boost/thread.hpp>

ContinuousThread::ContinuousThread(int wait_for_close) {
    this->wait_for_close = wait_for_close;
    closed = false;
}

ContinuousThread::~ContinuousThread() {
    close_signal = true;
    Log::debug("Oczekiwanie na zakończenie wątku...");
    for (int i = 0; i < wait_for_close && !closed; i++) {
        sleep_ms(1);
    }
    if (!closed) {
        Log::warn("Przekroczono czas oczekiwania - siłowe zamykanie wątku.");
        boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boost_thread);
        boost_thread2->interrupt();
        delete boost_thread2;
        boost_thread = nullptr;
    }
    Log::debug("Wątek zapętlonej procedury zakończony.");
}

void ContinuousThread::run() {
    while (!close_signal) {
        runLoop();
    }
    closed = true;
}

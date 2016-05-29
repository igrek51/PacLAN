#include "thread.h"
#include "../log.h"
#include "../system.h"

Thread::Thread() {
    init = false;
    close_signal = false;
    boost_thread = new boost::thread(boost::bind(&Thread::start, this));
    Log::info("Wątek utworzony.");
}

Thread::~Thread() {
    close_signal = true;
    if(boost_thread != nullptr) {
        boost_thread->interrupt();
        delete boost_thread;
        boost_thread = nullptr;
        Log::info("Wątek zniszczony.");
    }
}

void Thread::start(){
    while (!init) sleep_ms(1);
    Log::info("Wykonywanie głównej procedury wątku...");
    run();
    if (close_signal) {
        Log::info("Zakończono zdalnie główną procedurę wątku.");
    } else {
        Log::info("Samoistnie zakończono główną procedurę wątku.");
    }
}

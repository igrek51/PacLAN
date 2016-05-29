#include "thread.h"
#include "../log.h"
#include "../system.h"

Thread::Thread() {
    init = false;
    close_signal = false;
    boost_thread = new boost::thread(boost::bind(&Thread::start, this));
    log("Wątek utworzony.");
}

Thread::~Thread() {
    close_signal = true;
    if(boost_thread != nullptr) {
        boost_thread->interrupt();
        delete boost_thread;
        boost_thread = nullptr;
        log("Wątek zniszczony.");
    }
}

void Thread::start(){
    while (!init) sleep_ms(1);
    log("Wykonywanie głównej procedury wątku...");
    run();
    if (close_signal) {
        log("Zakończono zdalnie główną procedurę wątku.");
    } else {
        log("Samoistnie zakończono główną procedurę wątku.");
    }
}

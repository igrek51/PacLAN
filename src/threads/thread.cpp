#include "thread.h"
#include "../log/log.h"
#include "../utils.h"
#include <boost/thread.hpp>

Thread::Thread() {
    init = false;
    close_signal = false;
    boost_thread = reinterpret_cast<void*>(new boost::thread(boost::bind(&Thread::start, this)));
    Log::debug("Wątek utworzony.");
}

Thread::~Thread() {
    close_signal = true;
    if (boost_thread != nullptr) {
        boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boost_thread);
        boost_thread2->interrupt();
        delete boost_thread2;
        boost_thread = nullptr;
        Log::debug("Wątek zniszczony.");
    }
}

void Thread::start() {
    while (!init) sleep_ms(1);
    Log::debug("Wykonywanie głównej procedury wątku...");
    run();
    if (close_signal) {
        Log::debug("Zakończono zdalnie główną procedurę wątku.");
    } else {
        Log::debug("Samoistnie zakończono główną procedurę wątku.");
    }
}

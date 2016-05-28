/**
 * \file thread.h
 * Wątek z procedurą wykonywaną cyklicznie
 * \see thread.cpp
 */
#ifndef CONTINUOUS_THREAD_H
#define CONTINUOUS_THREAD_H

#include "thread.h"

class ContinuousThread : public Thread {
public:
    ContinuousThread(int wait_for_close = 0);
    virtual ~ContinuousThread();
protected:
    /// główna funkcja wątku
    virtual void run() = 0;
    /// powtarzana procedura wątku
    virtual void runLoop() = 0;
private:
    /// czas oczekiwania na zamknięcie
    int wait_for_close;
    /// czy zakończono wykonywanie funkcji run()
    volatile bool closed;
};

#endif

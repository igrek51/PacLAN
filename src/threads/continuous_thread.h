/**
 * \file continuous_thread.h
 * \see continuous_thread.cpp
 */
#ifndef CONTINUOUS_THREAD_H
#define CONTINUOUS_THREAD_H

#include "thread.h"

/**
 * \brief Wątek z procedurą wykonywaną cyklicznie
 */
class ContinuousThread : public Thread {
public:
    /**
     * konstruktor
     * @param wait_for_close czas oczekiwania na zamknięcie
     */
    ContinuousThread(int wait_for_close = 0);

    virtual ~ContinuousThread();

protected:
    /// główna funkcja wątku
    virtual void run();

    /// powtarzana procedura wątku
    virtual void runLoop() = 0;

private:
    /// czas oczekiwania na zamknięcie
    int wait_for_close;
    /// czy zakończono wykonywanie funkcji run()
    volatile bool closed;
};

#endif

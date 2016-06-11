/**
 * \file timer.h
 * \see timer.cpp
 */
#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include "continuous_thread.h"

/**
 * \brief Wątek odmierzania stałych odstępów czasu
 */
class Timer : private ContinuousThread {
public:
    /**
     * Utworzenie wątku timera
     * \param interval_ms interwał określający częstotliwość zwiększania licznika (w ms)
     * \param cycles wskaźnik na zmienną zwiększaną w każdym cyklu timera
     */
    Timer(int interval_ms, volatile int* cycles);

    /// zakończenie wątku timera
    ~Timer();

    /**
     * Zamiana sekund na cykle logiczne
     * \param s całkowita liczba sekund
     * \return całkowita liczba cykli logicznych
     */
    static int s_to_cycles(int s);

    /**
     * Zamiana cykli logicznych na sekundy
     * \param c całkowita liczba cykli logicznych
     * \return całkowita liczba sekund
     */
    static int cycles_to_s(int c);

private:
    /// interwał timera w milisekunach
    long long interval_ms;

    /// pętla timera zwiększająca licznik cykli co określony czas
    void runLoop();

    /// zapamiętany czas następnego wywołania pętli
    long long next_time;
    /// wskaźnik na zmienną zwiększaną w każdym cyklu timera
    volatile int* cycles;
};

#endif

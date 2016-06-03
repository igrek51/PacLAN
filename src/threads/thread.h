#ifndef THREAD_H
#define THREAD_H

//TODO ograniczyć includy z boosta, rozdzielić zależności, dostęp do App inny niż przez singleton

/**
 * \brief Klasa wątku
 */
class Thread {
public:
    /// uruchomienie nowego wątku
    Thread();

    /// zakończenie wątku
    virtual ~Thread();

protected:
    /// główna funkcja wykonywana w osobnym wątku
    virtual void run() = 0;

    /// czy zakończono inicjalizację
    volatile bool init;
    /// czy polecono zamknięcie
    volatile bool close_signal;
    /// wskaźnik do wątku (zrzutowany na void, żeby za każdym razem nie dołączać biblioteki boost, co znacznie wydłuża kompilację)
    void* boost_thread;
private:
    /// inicjalizacja uruchamiana w nowym wątku
    void start();
};

#endif

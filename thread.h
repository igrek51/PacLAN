/**
 * \file thread.h
 * Moduł obsługi wielu wątków
 * \see thread.cpp
 */
#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

/**
 * \brief Klasa wątku
 */
class Thread{
public:
    /// uruchomienie nowego wątku
    Thread();
    /// zakończenie wątku
    ~Thread();
    /// główna funkcja wątku
    virtual void run() = 0;
private:
    /// uchwyt do wątku
    HANDLE thread_handle;
};

#endif

/**
 * \file thread.h
 * Moduł obsługi wielu wątków
 * \see thread.cpp
 */
#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

DWORD WINAPI thread_start_routine(void *args);

/**
 * \brief Klasa wątku
 */
class Thread{
public:
    /// uruchomienie nowego wątku
    Thread();
    /// zakończenie wątku
    virtual ~Thread();
    /// główna funkcja wątku
    virtual void execute() = 0;
    /// czy zakończono inicjalizację
    volatile bool init;
    /// czy polecono zamknięcie
    volatile bool close;
protected:
    /// uchwyt do wątku
    HANDLE handle;
};


class SingleThread : public Thread {
public:
    virtual ~SingleThread();
    /// wykonywana jeden raz procedura wątku
    virtual void run() = 0;
private:
    void execute();
};

class ContinuousThread : public Thread {
public:
    ContinuousThread(int wait_for_close = 0);
    virtual ~ContinuousThread();
    /// powtarzana procedura wątku
    virtual void run() = 0;
protected:
    /// czas oczekiwania na zamknięcie
    int wait_for_close;
private:
    void execute();
    /// czy zakończono wykonywanie funkcji run()
    volatile bool closed;
};


class FileSearch : public SingleThread {
public:
    FileSearch();
    ~FileSearch();
    static volatile bool active;
private:
    void run();
};

#endif

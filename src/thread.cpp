#include "thread.h"
#include "log.h"

DWORD WINAPI thread_start_routine(void *args){
    Thread *thread = (Thread*)args;
    while(!thread->init) Sleep(1);
    log("Wykonywanie głównej procedury wątku...");
    thread->execute();
    if(thread->close){
        log("Zakończono zdalnie główną procedurę wątku.");
    }else{
        log("Samoistnie zakończono główną procedurę wątku.");
        delete thread;
    }
    ExitThread(0);
    return 0;
}

Thread::Thread(){
    init = false;
    close = false;
    DWORD watek_id;
    handle = CreateThread(NULL, 0, thread_start_routine, (void *)this, 0, &watek_id);
    log("Wątek utworzony.");
}

Thread::~Thread(){
    log("Wątek zniszczony.");
}


SingleThread::~SingleThread(){
    close = true;
    TerminateThread(handle, 0);
    CloseHandle(handle);
    log("Wątek jednorazowej procedury zakończony.");
}

void SingleThread::execute(){
    run();
}


ContinuousThread::ContinuousThread(int wait_for_close){
    this->wait_for_close = wait_for_close;
    closed = false;
}

ContinuousThread::~ContinuousThread(){
    close = true;
    log("Oczekiwanie na zakończenie wątku...");
    for(int i=0; i<wait_for_close && !closed; i++){
        Sleep(1);
    }
    if(!closed){
        log("Przekroczono czas oczekiwania - siłowe zamykanie wątku.");
        TerminateThread(handle, 0);
        CloseHandle(handle);
    }
    log("Wątek zapętlonej procedury zakończony.");
}

void ContinuousThread::execute(){
    while(!close){
        run();
    }
    closed = true;
}

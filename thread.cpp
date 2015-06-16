#include "thread.h"
#include "log.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    thread->run();
    log("Pętla wątku zakończona.");
    ExitThread(0);
    return 0;
}

Thread::Thread(){
    DWORD watek_id;
    running = true;
    thread_handle = CreateThread(NULL,0,watek,(void *)this,0,&watek_id);
}

Thread::~Thread(){
    running = false;
    Sleep(5); //odczekanie na zakończenie pętli wątku
    TerminateThread(thread_handle, 0);
    CloseHandle(thread_handle);
}

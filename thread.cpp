#include "thread.h"
#include "log.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    thread->run();
    ExitThread(0);
    return 0;
}

Thread::Thread(){
    DWORD watek_id;
    thread_handle = CreateThread(NULL,0,watek,(void *)this,0,&watek_id);
}

Thread::~Thread(){
    TerminateThread(thread_handle, 0);
    CloseHandle(thread_handle);
}

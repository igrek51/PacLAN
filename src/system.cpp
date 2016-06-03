#include "system.h"
//#include <boost/thread.hpp>
#include <unistd.h>
#include <sys/time.h>

void sleep_ms(int ms) {
    //boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
    usleep(1000 * ms);
}

long long currentMillis(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}
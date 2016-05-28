#include "system.h"
#include <boost/thread.hpp>

void sleep_ms(int ms) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

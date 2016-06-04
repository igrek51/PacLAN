#include "utils.h"
#include <unistd.h>
#include <sys/time.h>

void ss_clear(stringstream& sstream) {
    sstream.str("");
    sstream.clear();
}

bool has_extension(string filename, string ext) {
    if (ext.length() > filename.length())
        return false;
    filename = filename.substr(filename.length() - ext.length(), ext.length());
    return filename == ext;
}

char* string_to_char(string s) {
    char* result = new char[s.length() + 1];
    for (unsigned int i = 0; i < s.length(); i++) {
        result[i] = s[i];
    }
    result[s.length()] = 0;
    return result;
}

string itos(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

string get_time() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream ss2;
    if (timeinfo->tm_hour < 10) ss2 << "0";
    ss2 << timeinfo->tm_hour << ":";
    if (timeinfo->tm_min < 10) ss2 << "0";
    ss2 << timeinfo->tm_min << ":";
    if (timeinfo->tm_sec < 10) ss2 << "0";
    ss2 << timeinfo->tm_sec;
    return ss2.str();
}

int round_to_int(double d) {
    int c = (int) d;
    if (d - c >= 0.5)
        return c + 1;
    return c;
}


void sleep_ms(int ms) {
    usleep((__useconds_t) (1000 * ms));
}

long long currentMillis() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}
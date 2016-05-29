#ifndef LOG_H
#define LOG_H

#include <iostream>

using namespace std;

class Log {
public:
    /**
       @brief log_level Poziom wyświetlania logów
       0 - wyłączony
       1 - ERROR
       2 - WARN (+ ERROR)
       3 - INFO (+ WARN + ERROR)
       4 - DEBUG (wszystkie)
     */
    static int log_level;
    static int echo_level;
    static int errors_count;

    static const int OFF = 0;
    static const int ERROR = 1;
    static const int WARN = 2;
    static const int INFO = 3;
    static const int DEBUG = 4;

    static void error(string s);

    static void warn(string s);

    static void info(string s);

    static void debug(string s);

    static bool isError();

    /// wyczyszczenie zawartości pliku dziennika
    static void logClear();

    /**
     * wyświetlenie komunikatu o błędzie
     * @param s treść komunikatu
     */
    static void criticalError(string s);

private:
    static void log(string s, int level);

    static void echo(string s, int level);
};

#endif

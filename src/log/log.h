#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <list>
#include "log_level.h"

using namespace std;

class Log {
public:
    static void error(string s);

    static void warn(string s);

    static void info(string s);

    static void debug(string s);

    static bool wasCriticalError();

    /**
     * wyświetlenie komunikatu o błędzie
     * @param s treść komunikatu
     */
    static void criticalError(string s);

    static list<string>* getEchoes();

private:
    static Log* instance;

    static Log* getInstance();

    int criticalErrorsCount;

    list<string> echoes;

    Log();

    ~Log();

    void log(string s, LogLevel level);

    void echo(string s, LogLevel level);

    /// wyczyszczenie zawartości pliku dziennika
    void clearLog();
};

#endif

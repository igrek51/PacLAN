#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

enum class LogLevel : int {
    OFF = 0, //wyłączony
    ERROR = 1, //ERROR
    WARN = 2, //WARN + ERROR
    INFO = 3, //INFO + WARN + ERROR
    DEBUG = 4 //wszystkie
};

#endif
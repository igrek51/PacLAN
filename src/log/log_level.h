/**
 * \file log_level.h
 * definicja poziomów loga
 * \see log_level.cpp
 */
#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

/**
 * poziom komunikatu loga
 */
enum class LogLevel : int {
    OFF = 0, //wyłączony
    ERROR = 1, //ERROR
    WARN = 2, //WARN + ERROR
    INFO = 3, //INFO + WARN + ERROR
    DEBUG = 4 //wszystkie
};

#endif
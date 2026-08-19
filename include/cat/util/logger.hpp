#pragma once

// @file logger is a wrapper to std::std::cout
// when DEVELOP macro is not set, all logger macros are removed

#ifdef DEVELOP

#include <iostream>     // IWYU pragma: export
#include <format>       // IWYU pragma: export

#define LOG_TEXT(_msg, ...)                                                 \
    do {                                                                    \
        std::string _text = std::format(_msg __VA_OPT__(,) __VA_ARGS__);    \
        std::cout << _text << "\n";                                         \
        }                                                                   \
    while (false)   

#define LOG_ERR(_msg, ...)                                                  \
    do {                                                                    \
        std::string _text = std::format(_msg __VA_OPT__(,) __VA_ARGS__);    \
        std::cerr << _text << "\n";                                         \
        }                                                                   \
    while (false)   

#else

#define LOG_TEXT(_msg)
#define LOG_ERR(_msg) 

#define LOG_TEXTF(_msg, ...)
#define LOG_ERRF(_msg, ...)

#endif

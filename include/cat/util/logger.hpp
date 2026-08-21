#pragma once

// @file logger is a wrapper to std::std::cout
// when DEVELOP macro is not set, all logger macros are removed

#ifdef DEVELOP

#include <iostream>     // IWYU pragma: export
#include <format>       // IWYU pragma: export

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

#define LOG_TEXT(_msg, ...)                                                 \
    do {                                                                    \
        std::string _text = std::format(_msg __VA_OPT__(,) __VA_ARGS__);    \
        std::cout << _text << "\n";                                         \
        }                                                                   \
    while (false)   

#define LOG_ERR(_msg, ...)                                                  \
    do {                                                                    \
        std::string _text = std::format(_msg __VA_OPT__(,) __VA_ARGS__);    \
        std::cerr << BOLD << RED << _text << RESET << "\n";                 \
        }                                                                   \
    while (false)   

#define LOG_WARN(_msg, ...)                                                 \
    do {                                                                    \
        std::string _text = std::format(_msg __VA_OPT__(,) __VA_ARGS__);    \
        std::cerr << BOLD << YELLOW << _text << RESET << "\n";              \
        }                                                                   \
    while (false)   

#else

#define LOG_TEXT(...)
#define LOG_ERR(...) 
#define LOG_WARN(...)

#endif

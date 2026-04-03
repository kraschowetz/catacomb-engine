#pragma once

// @file logger is a wrapper to std::std::cout
// when DEVELOP macro is not set, all logger macros are removed

#ifdef DEVELOP

#include <cstdio>

#define LOG_TEXT(_msg) printf(_msg)
#define LOG_ERR(_msg) fprintf(stderr, _msg);

#define LOG_TEXTF(_msg, ...) printf(_msg, __VA_ARGS__);
#define LOG_ERRF(_msg, ...) fprintf(stderr, _msg, __VA_ARGS__)

#else

#define LOG_TEXT(_msg)
#define LOG_ERR(_msg) 

#define LOG_TEXTF(_msg, ...)
#define LOG_ERRF(_msg, ...)

#endif

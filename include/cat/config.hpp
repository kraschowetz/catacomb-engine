#pragma once

#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef u8 byte_t;
typedef u64 hash_t;

#define THROWS noexcept(false)
#define IN
#define OUT

#ifdef DEVELOP

#include <iostream>     // IWYU pragma: export
#include <format>       // IWYU pragma: export

#define DEBUGBREAK __builtin_trap()

#define FMT_MESSAGE(msg, ...) std::format(msg __VA_OPT__(,) __VA_ARGS__)

#define ASSERT(expr, ...)                                                   \
    do {                                                                    \
        if(!(expr))                                                         \
        {                                                                   \
            std::cerr << "assertion failed! (" << #expr << ")\n";           \
            std::cerr << "\t at line " << __LINE__ << " of "                \
                << __FILE__ << "\n";                                        \
            __VA_OPT__(std::string errmsg = FMT_MESSAGE(__VA_ARGS__);)      \
            __VA_OPT__(std::cerr << errmsg << "\n";)                        \
            std::abort();                                                   \
        }                                                                   \
    } while(false)

#else

#define DEBUGBREAK
#define ASSERT(...) do{}while(false)

#endif

#ifndef LOG_ENGINE_SHADER_WARNINGS
    #define LOG_ENGINE_SHADER_WARNINGS 0
#endif

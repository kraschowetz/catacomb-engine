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

#define THROWS noexcept(false)
#define IN
#define OUT

#ifdef DEVELOP

#define DEBUGBREAK __builtin_debugtrap()
#define CAT_ASSERT(expr) if(!(expr)) DEBUGBREAK

#else

#define DEBUGBREAK
#define CAT_ASSERT(expr)

#endif

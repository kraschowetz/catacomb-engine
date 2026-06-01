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

// resource loader utils
#define CAT_RESOURCELOADER_LOAD_DECL(Type, ...)         \
    hash_t operator()(Type* dest, __VA_ARGS__) const THROWS

#define CAT_RESOURCELOADER_HASH_DECL(...)               \
    hash_t operator()(__VA_ARGS__) const noexcept

#define CAT_RESOURCELOADER_UNLOAD_DECL(Type)            \
    void operator()(Type*) const

// args should be: Type* dest, ...
#define CAT_RESOURCELOADER_LOAD_IMPL(...)               \
    operator()(__VA_ARGS__) const THROWS

#define CAT_RESOURCELOADER_HASH_IMPL(...)               \
    operator()(__VA_ARGS__) const noexcept

// args should be: Type*
#define CAT_RESOURCELOADER_UNLOAD_IMPL(...)            \
    operator()(__VA_ARGS__) const

#ifdef DEVELOP

#define DEBUGBREAK __builtin_trap()
#define CAT_ASSERT(expr) if(!(expr)) DEBUGBREAK

#else

#define DEBUGBREAK
#define CAT_ASSERT(expr)

#endif

#pragma once

#include <new>
#include <string>
#include <unordered_map>

#include <cat/config.hpp>
#include <variant>

namespace cat
{

enum class eTypeID : u8
{
    F32,
    I32,
    U32,
    U64,
    BOOL,
    STR
};

struct BasicConfEntry
{
public:
    std::variant<
        f32,
        i32,
        u32,
        u64,
        bool,
        std::string
    > data;

public:
    BasicConfEntry() = default;

    BasicConfEntry(f32 v) : data{v} {}
    BasicConfEntry(i32 v) : data{v} {}
    BasicConfEntry(u32 v) : data{v} {}
    BasicConfEntry(u64 v) : data{v} {}
    BasicConfEntry(bool v) : data{v} {}
    BasicConfEntry(const std::string& v) : data{v} {}
};

using BasicConfMap = std::unordered_map<std::string, BasicConfEntry>;

BasicConfMap load_conf_file(const std::string& path) THROWS;

}

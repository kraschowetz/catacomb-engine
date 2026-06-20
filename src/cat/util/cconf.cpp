#include <cat/util/cconf.hpp>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

#include <cat/error.hpp>
#include <cat/util/util.hpp>

namespace cat
{

static eTypeID _get_type_id(const std::string& type_str)
{
    hash_t type_hash = str_hash(type_str);
    
    switch (type_hash)
    {
        case str_hash("i32"): return eTypeID::I32;
        case str_hash("u32"): return eTypeID::U32;
        case str_hash("u64"): return eTypeID::U64;
        case str_hash("f32"): return eTypeID::F32;
        case str_hash("bool"): return eTypeID::BOOL;
        case str_hash("str"): return eTypeID::STR;
        default: 
            LOG_ERR("invalid type\n"); 
            return eTypeID::STR;
    }
}

static BasicConfEntry _parse_value(eTypeID type, const std::string& value)
{
    switch(type)
    {
        case eTypeID::I32: return (i32)std::atoi(value.c_str());
        case eTypeID::U32: return (u32)std::strtoul(value.c_str(), nullptr, 10);
        case eTypeID::U64: return (u64)std::strtoull(value.c_str(), nullptr, 10);
        case eTypeID::F32: return (f32)std::atof(value.c_str());
        case eTypeID::BOOL: return (bool)std::atoi(value.c_str()) != 0;
        case eTypeID::STR: return value;
    }
}

BasicConfMap load_conf_file(const std::string &path) THROWS
{
    std::ifstream file{path};

    if(!file.is_open())
    {
        throw Exception{eErrorCode::FILE_NOT_FOUND};
    }

    std::string current_line;
    BasicConfMap map;

    while(std::getline(file, current_line))
    {
        if(current_line.length() <= 1) continue;

        std::string name = std::strtok(
            const_cast<char*>(current_line.c_str()), ";"
        );
        std::string type = std::strtok(
            NULL, ";"
        );
        std::string value = std::strtok(
            NULL, ";"
        );

        map[name] = _parse_value(_get_type_id(type), value);
    }

    return map;
}

}

#include <cat/util/cconf.hpp>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include <cat/error.hpp>
#include <cat/util/util.hpp>

namespace cat
{

static constexpr std::string _get_entry_type_str(const BasicConfEntry& entry)
{
    switch (entry.data.index())
    {
        case 0: return "f32";
        case 1: return "i32";
        case 2: return "u32";
        case 3: return "u64";
        case 4: return "bool";
        default: return "str";
    }
}

static constexpr std::string _get_entry_value_str(const BasicConfEntry& entry)
{
    switch (entry.data.index())
    {
        case 0: return std::to_string(std::get<f32>(entry.data));
        case 1: return std::to_string(std::get<i32>(entry.data));
        case 2: return std::to_string(std::get<u32>(entry.data));
        case 3: return std::to_string(std::get<u64>(entry.data));
        case 4: return std::to_string(std::get<bool>(entry.data));
        default: return std::get<std::string>(entry.data);
    }
}

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
            const_cast<char*>(current_line.c_str()), CAT_CCONF_DELIMETER
        );
        std::string type = std::strtok(
            NULL, CAT_CCONF_DELIMETER
        );
        std::string value = std::strtok(
            NULL, CAT_CCONF_DELIMETER
        );

        map[name] = _parse_value(_get_type_id(type), value);
    }

    return map;
}

void save_conf_file(const BasicConfMap &map, const std::string &path)
{
    std::ofstream file{path};
    std::stringstream stream;

    for(std::pair<std::string, BasicConfEntry> pair : map)
    {
        stream << pair.first << CAT_CCONF_DELIMETER;
        stream << _get_entry_type_str(pair.second) << CAT_CCONF_DELIMETER;
        stream << _get_entry_value_str(pair.second) << CAT_CCONF_END_DELIMETER;
    }

    file << stream.str();
    file.close();
}

}

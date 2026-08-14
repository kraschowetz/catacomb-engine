#pragma once

#include <cat/util/util.hpp>

#include "glaze/json/generic_fwd.hpp"
#include <string>
#include <unordered_map>
namespace cat
{

// maps each entity component name to its json object
using EntityJson = std::unordered_map<std::string, glz::generic>;

struct SceneJson
{
    struct 
    {
        std::string name;
    } scene_info;

    std::vector<EntityJson> entities;
};

SceneJson read_scene_json(const std::string& path) THROWS;

}

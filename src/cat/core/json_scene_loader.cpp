#include <cat/core/json_scene_loader.hpp>

#include "glaze/json/read.hpp"

namespace cat
{

SceneJson read_scene_json(const std::string& path) THROWS
{
    SceneJson scene;

    auto ec = glz::read_file_json(scene, path, std::string{});

    if(ec) throw Exception{eErrorCode::FAILED};

    return scene;
}

}

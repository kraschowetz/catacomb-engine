#include <cat/gfx/components/c_sprite.hpp>

#include <cat/gfx/texture_loader.hpp>
#include "cat/gfx/sprite_atlas.hpp"
#include "cat/core/core_engine.hpp"
#include "cat/core/resource_manager.hpp"
#include "glm/fwd.hpp"

namespace cat 
{

f32 convert_z_index_to_position(u8 z_index)
{
    if(z_index == 0) return 0.f;

    return 1.f / (f32) z_index;
}

cSprite cSprite::from_json(const glz::generic& json)
{
    const auto& uv_json   = json["uv"];
    const auto& path_json = json["path"];
    const auto& size_json = json["size"];
    const auto& z_json    = json["z_index"];

    CAT_ASSERT(uv_json.is_array());
    CAT_ASSERT(path_json.is_string());
    CAT_ASSERT(size_json.is_array());
    CAT_ASSERT(z_json.is_number());

    Shared<Texture> texture = CoreEngine::get().get_resource_manager()
            .load<Texture, TextureLoader>(path_json.get<std::string>());

    TextureUV uv = {
        (f32) uv_json[0][0].get<f64>(),
        (f32) uv_json[0][1].get<f64>(),
        (f32) uv_json[1][0].get<f64>(),
        (f32) uv_json[1][1].get<f64>(),
    };

    glm::ivec2 size = {
        (i32) size_json[0].get<double>(),
        (i32) size_json[1].get<double>()
    };

    return cSprite{
        .uv = uv,
        .size = size,
        .texture_handle = texture->get_handle(),
        .z_index = json["z_index"].as<u8>()
    };
}

}

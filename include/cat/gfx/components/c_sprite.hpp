#pragma once

#include <cat/config.hpp>
#include "cat/gfx/texture.hpp"
#include "cat/util/memory.hpp"
#include <cat/core/component_io.hpp>

namespace cat
{

struct cSprite
{
    TextureUV uv;
    glm::ivec2 size;
    Shared<Texture> texture;
    Shared<class Shader> shader = nullptr;
    u8 z_index;

    static cSprite from_json(const glz::generic&);
};

f32 convert_z_index_to_position(u8 z_index);

}

#pragma once

#include <cat/config.hpp>
#include "cat/gfx/texture.hpp"

namespace cat
{

struct cSprite
{
    TextureUV uv;
    glm::ivec2 size;
    u32 texture_handle;
    u8 z_index;
};

f32 convert_z_index_to_position(u8 z_index);

}

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
    u32 z_index;
};

}

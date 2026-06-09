#pragma once

#include <cat/gfx/components/c_sprite.hpp>  // IWYU pragma: export
#include <cat/gfx/texture.hpp>              // IWYU pragma: export
#include <cat/util/memory.hpp>

namespace cat
{

struct SpriteAtlas 
{
public:
    SpriteAtlas(const Shared<Texture>&, const glm::ivec2& sprite_size);

    TextureUV get_uv(const glm::ivec2& sprite_index) const;
    cSprite get_sprite(const glm::ivec2& index, u32 z_index = 0) const;

private:
    glm::ivec2 m_sprite_size;
    Shared<Texture> m_texture;
};

};

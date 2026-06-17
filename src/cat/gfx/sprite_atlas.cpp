#include "cat/gfx/sprite_atlas.hpp"
#include "cat/util/logger.hpp"

using namespace cat;

SpriteAtlas::SpriteAtlas(const Shared<Texture>& texture, const glm::ivec2& sprite_size)
    : m_sprite_size(sprite_size)
    , m_texture(texture)
{}

TextureUV SpriteAtlas::get_uv(const glm::ivec2& sprite_index) const
{
    TextureUV uv;
    glm::ivec2 texture_size = m_texture->get_size();

    uv.top_y = (f32)(sprite_index.y * m_sprite_size.y) / (f32)texture_size.y;
    uv.bottom_y = (f32)((sprite_index.y + 1) * m_sprite_size.y) / (f32)texture_size.y;

    uv.left_x = (f32)(sprite_index.x * m_sprite_size.x) / (f32)texture_size.x;
    uv.right_x = (f32)((sprite_index.x + 1) * m_sprite_size.x) / (f32)texture_size.x;

    LOG_TEXTF("texture size: %dx%d\n", texture_size.x, texture_size.y);
    LOG_TEXTF("spriteid: %d, %d\n", sprite_index.x, sprite_index.y);
    LOG_TEXTF("left: %.2f, right: %.2f\n", uv.left_x, uv.right_x);
    LOG_TEXTF("bottom: %.2f, top: %.2f\n", uv.bottom_y, uv.top_y);

    return uv;
}

cSprite SpriteAtlas::get_sprite(const glm::ivec2& index, u32 z_index) const
{
    return cSprite
    {
        .uv = get_uv(index),
        .size = m_sprite_size,
        .texture_handle = m_texture->get_handle(),
        .z_index = z_index
    };
}

void SpriteAtlas::bind(u32 unit) const
{
    m_texture->bind(unit);
}

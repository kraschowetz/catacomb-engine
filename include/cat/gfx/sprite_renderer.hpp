#pragma once

#include "cat/util/util.hpp"
#include <cat/gfx/components/c_sprite.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/core/components/c_transform.hpp>
#include <cat/util/memory.hpp>
#include <vector>

namespace cat
{

class SpriteRenderer
{
public:
    SpriteRenderer();

    NO_COPY(SpriteRenderer);

    void render_sprite(const cSprite& sprite, const cTransform& transform);
    bool has_sprites_batched() const;

private:
    void add_sprite_to_batch(const cSprite& sprite, const cTransform& transform);
    void render_batch();

private:
    Unique<VertexBuffer> m_vbo;
    Unique<VertexBuffer> m_ibo;
    Unique<VertexArray> m_vao;
    
    std::vector<f32> m_batch_position_data;
    std::vector<f32> m_batch_uv_data;
    u32 m_current_spriteatlas_handle;
    u32 m_num_sprites_batched = 0;

    static constexpr u32 SPRITE_BATCH_SIZE = 16;
    static constexpr u32 SPRITE_VERTEX_SIZE = (sizeof(f32)*3)+(sizeof(f32)*2);

    friend class GfxEngine;
};

}

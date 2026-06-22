#pragma once

#include "cat/gfx/vertex_buffer.hpp"
#include "cat/gfx/vertex_layout.hpp"
#include "cat/util/util.hpp"
#include <cat/gfx/components/c_sprite.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/core/components/c_transform.hpp>
#include <cat/core/components/c_world_transform.hpp>
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
    void render_sprite(const cSprite& sprite, const cWorldTransform& transform);
    bool has_sprites_batched() const;

private:
    void add_sprite_to_batch(const cSprite& sprite, const glm::mat4& model);
    void render_batch();

private:
    static constexpr u64 BUFFER_RING_SIZE = 3;

    std::array<Unique<VertexBuffer>, BUFFER_RING_SIZE> m_vbo_ring;
    std::array<Unique<VertexArray>, BUFFER_RING_SIZE> m_vao_ring;
    u32 m_ring_index = 0;

    Unique<VertexBuffer> m_ibo;
    VertexLayout m_sprite_layout;
    
    std::vector<f32> m_batch_position_data;
    std::vector<f32> m_batch_uv_data;
    u32 m_current_spriteatlas_handle;
    u32 m_num_sprites_batched = 0;

    static constexpr u32 SPRITE_BATCH_SIZE = 64;
    static constexpr u32 SPRITE_VERTEX_SIZE = (sizeof(f32)*3)+(sizeof(f32)*2);


    friend class GfxEngine;
};

}

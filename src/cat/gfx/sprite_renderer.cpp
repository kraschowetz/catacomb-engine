#include "cat/error.hpp"
#include <cat/gfx/sprite_renderer.hpp>

#include <cat/gfx/gfx_engine.hpp>
#include <cat/gfx/vertex_buffer.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/vertex_layout.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <memory>

using namespace cat;

static constexpr u32 SPRITE_VERTEX_COUNT = 4;
static constexpr u32 SPRITE_INDEX_COUNT = 6;

SpriteRenderer::SpriteRenderer()
{
    if(!GfxEngine::is_loaded())
    {
        GfxEngine::get();
        throw Exception{eErrorCode::UNKNOWN};
    }

    m_vbo = std::make_unique<VertexBuffer>(
        SPRITE_VERTEX_SIZE,
        SPRITE_BATCH_SIZE * SPRITE_VERTEX_COUNT,
        eBufferType::VERTEX
    );

    m_ibo = std::make_unique<VertexBuffer>(
        sizeof(u32),
        SPRITE_BATCH_SIZE * SPRITE_INDEX_COUNT,
        eBufferType::INDEX
    );

    m_vao = std::make_unique<VertexArray>();

    m_batch_position_data.reserve(
        SPRITE_BATCH_SIZE * SPRITE_VERTEX_COUNT * 3);
    m_batch_uv_data.reserve(
        SPRITE_BATCH_SIZE * SPRITE_VERTEX_COUNT * 2);

    // populate ibo
    std::array<u32, SPRITE_BATCH_SIZE * SPRITE_INDEX_COUNT> indices;
    for(u32 i = 0; i < SPRITE_BATCH_SIZE; ++i)
    {
        u32 base_vertex = i * SPRITE_VERTEX_COUNT;
        u32 base_index  = i * SPRITE_INDEX_COUNT;

        indices[base_index + 0] = base_vertex + 0;
        indices[base_index + 1] = base_vertex + 1;
        indices[base_index + 2] = base_vertex + 2;
        indices[base_index + 3] = base_vertex + 2;
        indices[base_index + 4] = base_vertex + 3;
        indices[base_index + 5] = base_vertex + 0;
    }

    m_ibo->upload_indices(indices.data(), SPRITE_BATCH_SIZE * SPRITE_INDEX_COUNT);
}

void SpriteRenderer::render_sprite(const cSprite &sprite, const cTransform& transform)
{
    constexpr u64 BATCH_CAPACITY = 
        SPRITE_BATCH_SIZE * SPRITE_VERTEX_COUNT * SPRITE_VERTEX_SIZE;

    if(m_num_sprites_batched >= BATCH_CAPACITY ||
        sprite.texture_handle != m_current_spriteatlas_handle)
    {
        render_batch();
    }

    add_sprite_to_batch(sprite, transform);
}

bool SpriteRenderer::has_sprites_batched() const
{
    return m_num_sprites_batched != 0;
}

void SpriteRenderer::add_sprite_to_batch(
    const cSprite& sprite, const cTransform& transform)
{
    glm::ivec2 size = glm::ivec2{
        (i32)((f32)(sprite.size.x) * transform.scale.x),
        (i32)((f32)(sprite.size.y) * transform.scale.y),
    };

    // TODO: check if manual memcpys would perform better

    // bottom-left
    m_batch_position_data.push_back(transform.position.x - (f32) size.x);
    m_batch_position_data.push_back(transform.position.y - (f32) size.y);
    m_batch_position_data.push_back((f32) sprite.z_index);

    m_batch_uv_data.push_back(sprite.uv.left_x);
    m_batch_uv_data.push_back(sprite.uv.bottom_y);

    // bottom-right
    m_batch_position_data.push_back(transform.position.x + (f32) size.x);
    m_batch_position_data.push_back(transform.position.y - (f32) size.y);
    m_batch_position_data.push_back((f32) sprite.z_index);

    m_batch_uv_data.push_back(sprite.uv.right_x);
    m_batch_uv_data.push_back(sprite.uv.bottom_y);

    // top-right
    m_batch_position_data.push_back(transform.position.x + (f32) size.x);
    m_batch_position_data.push_back(transform.position.y + (f32) size.y);
    m_batch_position_data.push_back((f32) sprite.z_index);

    m_batch_uv_data.push_back(sprite.uv.right_x);
    m_batch_uv_data.push_back(sprite.uv.top_y);

    // top-left
    m_batch_position_data.push_back(transform.position.x - (f32) size.x);
    m_batch_position_data.push_back(transform.position.y + (f32) size.y);
    m_batch_position_data.push_back((f32) sprite.z_index);

    m_batch_uv_data.push_back(sprite.uv.left_x);
    m_batch_uv_data.push_back(sprite.uv.top_y);

    ++m_num_sprites_batched;
}

void SpriteRenderer::render_batch()
{
    VertexLayout layout;
    layout.push_f32(3); // position
    layout.push_f32(2); // uv

    m_vbo->buffer(m_batch_position_data.data(), layout, 0);
    m_vbo->buffer(m_batch_uv_data.data(), layout, 1);

    m_vao->attr(*m_vbo, layout);

    m_vao->bind();
    m_ibo->bind();

    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        (i32)(m_num_sprites_batched * SPRITE_INDEX_COUNT),
        GL_UNSIGNED_INT,
        nullptr
    ));

    m_ibo->unbind();
    m_vao->unbind();

    m_num_sprites_batched = 0;

    m_batch_position_data.clear();
    m_batch_uv_data.clear();
}

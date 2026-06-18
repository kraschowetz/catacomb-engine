#include "cat/error.hpp"
#include "cat/util/memory.hpp"
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

    m_sprite_layout.push_f32(3); // position
    m_sprite_layout.push_f32(2); // uv

    for(u64 i = 0; i < BUFFER_RING_SIZE; ++i)
    {
        m_vbo_ring[i] = std::make_unique<VertexBuffer>(
            SPRITE_VERTEX_SIZE,
            SPRITE_BATCH_SIZE * SPRITE_VERTEX_COUNT,
            eBufferType::VERTEX,
            eBufferUsage::DYNAMIC
        );
        m_vao_ring[i] = std::make_unique<VertexArray>();
        
        m_vao_ring[i]->bind();
        m_vbo_ring[i]->bind();

        m_vao_ring[i]->attr(*m_vbo_ring[i], m_sprite_layout);

        m_vao_ring[i]->unbind();
        m_vbo_ring[i]->unbind();
    }

    m_ibo = std::make_unique<VertexBuffer>(
        sizeof(u32),
        SPRITE_BATCH_SIZE * SPRITE_INDEX_COUNT,
        eBufferType::INDEX
    );

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

    m_ibo->bind();
    m_ibo->upload_indices(indices.data(), SPRITE_BATCH_SIZE * SPRITE_INDEX_COUNT);
    m_ibo->unbind();
}

void SpriteRenderer::render_sprite(const cSprite &sprite, const cTransform& transform)
{
    if(m_num_sprites_batched >= SPRITE_BATCH_SIZE ||
        sprite.texture_handle != m_current_spriteatlas_handle)
    {
        render_batch();
    }

    m_current_spriteatlas_handle = sprite.texture_handle;
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
    size /= 2;

    const glm::vec2 anchor {
        transform.position.x,
        transform.position.y
    };

    const glm::vec2 half_size {
        (f32)sprite.size.x * 0.5f,
        (f32)sprite.size.y * 0.5f
    };

    glm::mat4 model = cTransform::as_mat4(transform);

    auto _transform_corner = [&](f32 _x, f32 _y) -> glm::vec2 {
        glm::vec4 world = model * glm::vec4(_x, _y, 0.f, 1.f);
        return glm::vec2{world.x, world.y};
    };

    glm::vec2 bottom_left = _transform_corner(-half_size.x, -half_size.y);
    glm::vec2 bottom_right = _transform_corner(half_size.x, -half_size.y);
    glm::vec2 top_right = _transform_corner(half_size.x, half_size.y);
    glm::vec2 top_left = _transform_corner(-half_size.x, half_size.y); 

    u64 i = m_batch_position_data.size();
    u64 j = m_batch_uv_data.size();

    constexpr u64 NUM_POSITIONS = 12;
    constexpr u64 NUM_UVS = 8;

    m_batch_position_data.resize(i + NUM_POSITIONS);
    m_batch_uv_data.resize(j + NUM_UVS);

    m_batch_position_data[i] = bottom_left.x;
    m_batch_position_data[i+1] = bottom_left.y;
    m_batch_position_data[i+2] = (f32) sprite.z_index;

    m_batch_uv_data[j] = sprite.uv.left_x;
    m_batch_uv_data[j+1] = sprite.uv.bottom_y;

    // bottom-right
    m_batch_position_data[i+3] = bottom_right.x;
    m_batch_position_data[i+4] = bottom_right.y;
    m_batch_position_data[i+5] = (f32) sprite.z_index;

    m_batch_uv_data[j+2] = sprite.uv.right_x;
    m_batch_uv_data[j+3] = sprite.uv.bottom_y;

    // top-right
    m_batch_position_data[i+6] = top_right.x;
    m_batch_position_data[i+7] = top_right.y;
    m_batch_position_data[i+8] = (f32) sprite.z_index;

    m_batch_uv_data[j+4] = sprite.uv.right_x;
    m_batch_uv_data[j+5] = sprite.uv.top_y;

    // top-left
    m_batch_position_data[i+9] = top_left.x;
    m_batch_position_data[i+10] = top_left.y;
    m_batch_position_data[i+11] = (f32) sprite.z_index;

    m_batch_uv_data[j+6] = sprite.uv.left_x;
    m_batch_uv_data[j+7] = sprite.uv.top_y;

    ++m_num_sprites_batched;
}

void SpriteRenderer::render_batch()
{
    const VertexBuffer& vbo = *m_vbo_ring[m_ring_index];
    const VertexArray& vao = *m_vao_ring[m_ring_index];

    vbo.bind();
    vao.bind();

    vbo.buffer(m_batch_position_data.data(), m_sprite_layout, 0);
    vbo.buffer(m_batch_uv_data.data(), m_sprite_layout, 1);

    // unbinds m_vbo
    m_ibo->bind();

    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        (i32)(m_num_sprites_batched * SPRITE_INDEX_COUNT),
        GL_UNSIGNED_INT,
        nullptr
    ));

    m_ibo->unbind();
    vao.unbind();

    m_num_sprites_batched = 0;

    m_batch_position_data.clear();
    m_batch_uv_data.clear();

    m_ring_index = (m_ring_index + 1) % BUFFER_RING_SIZE;
}

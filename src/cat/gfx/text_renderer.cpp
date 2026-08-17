#include <cat/gfx/text_renderer.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <memory>

#include "cat/core/components/c_transform.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include <cat/gfx/gfx_util.hpp>
#include "cat/gfx/glyph_quad.hpp"
#include "cat/gfx/sprite_renderer.hpp"
#include "cat/gfx/vertex_array.hpp"
#include "cat/gfx/vertex_buffer.hpp"

using namespace cat;

TextRenderer::TextRenderer()
{
    CAT_ASSERT(GfxEngine::is_loaded());

    m_glyph_layout.push_f32(3); // position
    m_glyph_layout.push_f32(2); // uv

    for(u64 i = 0; i < BUFFER_RING_SIZE; ++i)
    {
        m_vbo_ring[i] = std::make_unique<VertexBuffer>(
            GLYPH_VERTEX_SIZE,
            GLYPH_BATCH_SIZE * GLYPH_VERTEX_COUNT,
            eBufferType::VERTEX,
            eBufferUsage::DYNAMIC
        );
        m_vao_ring[i] = std::make_unique<VertexArray>();

        m_vao_ring[i]->bind();
        m_vbo_ring[i]->bind();

        m_vao_ring[i]->attr(*m_vbo_ring[i], m_glyph_layout);

        m_vao_ring[i]->unbind();
        m_vbo_ring[i]->unbind();
    }

    m_ibo = std::make_unique<VertexBuffer>(
        sizeof(u32),
        GLYPH_BATCH_SIZE * GLYPH_INDEX_COUNT,
        eBufferType::INDEX
    );

    m_batch_position_data.reserve(
        GLYPH_BATCH_SIZE * GLYPH_VERTEX_COUNT * 3
    );
    m_batch_uv_data.reserve(
        GLYPH_BATCH_SIZE * GLYPH_VERTEX_COUNT * 2
    );

    std::array<u32, GLYPH_BATCH_SIZE * GLYPH_INDEX_COUNT> indices;
    for(u32 i = 0; i < GLYPH_BATCH_SIZE; ++i)
    {
        const u32 base_vertex = i * GLYPH_VERTEX_COUNT;
        const u32 base_index = i * GLYPH_INDEX_COUNT;

        indices[base_index + 0] = base_vertex + 0;
        indices[base_index + 1] = base_vertex + 1;
        indices[base_index + 2] = base_vertex + 2;
        indices[base_index + 3] = base_vertex + 2;
        indices[base_index + 4] = base_vertex + 3;
        indices[base_index + 5] = base_vertex + 0;
    }

    m_ibo->bind();
    m_ibo->upload_indices(indices.data(), indices.max_size());
    m_ibo->unbind();
}

void TextRenderer::render_text(const cText& text, const cTransform& transform)
{
    u32 handle = text.get_font()->get_atlas()->get_handle();
    if(
        m_num_glyphs_batched + text.get_content().size() >= GLYPH_BATCH_SIZE    ||
        text.get_color() != m_current_ink_color                                 ||
        handle != m_current_font_handle
    )
    {
        render_batch();

        m_current_font_handle = handle;
        m_current_ink_color = text.get_color();
    }

    std::vector<GlyphQuad> layout = layout_text(
        text.get_content(),
        *text.get_font(),
        text.get_font_size()
    );

    for(const GlyphQuad& quad : layout)
    {
        glm::mat4 model = transform.as_mat4();
        model *= glm::translate(glm::mat4{1.f}, glm::vec3{quad.position, 0.f});
        add_glyph_to_batch(quad, transform.position);
    }
}

void TextRenderer::add_glyph_to_batch(const GlyphQuad& glyph, const glm::vec3& position)
{
    constexpr u64 NUM_POSITIONS = 12;
    constexpr u64 NUM_UVS = 8;

    const f32 z_index = 0.0f;

    u64 i = m_batch_position_data.size();
    u64 j = m_batch_uv_data.size();

    const f32 left = position.x + glyph.position.x - (glyph.size.x / 2.f);
    const f32 right = position.x + glyph.position.x + (glyph.size.x / 2.f);
    const f32 top = position.y + glyph.position.y + (glyph.size.y / 2.f);
    const f32 bottom = position.y + glyph.position.y - (glyph.size.y / 2.f);

    m_batch_position_data.resize(i + NUM_POSITIONS);
    m_batch_uv_data.resize(j + NUM_UVS);

    // bottom left
    m_batch_position_data[i] = left;
    m_batch_position_data[i+1] = bottom;
    m_batch_position_data[i+2] = z_index;

    m_batch_uv_data[j] = glyph.uv_min.x;
    m_batch_uv_data[j+1] = glyph.uv_min.y;

    // bottom right
    m_batch_position_data[i+3] = right;
    m_batch_position_data[i+4] = bottom;
    m_batch_position_data[i+5] = z_index;

    m_batch_uv_data[j+2] = glyph.uv_max.x;
    m_batch_uv_data[j+3] = glyph.uv_min.y;

    // top right
    m_batch_position_data[i+6] = right;
    m_batch_position_data[i+7] = top;
    m_batch_position_data[i+8] = z_index;

    m_batch_uv_data[j+4] = glyph.uv_max.x;
    m_batch_uv_data[j+5] = glyph.uv_max.y;

    // top left
    m_batch_position_data[i+9] = left;
    m_batch_position_data[i+10] = top;
    m_batch_position_data[i+11] = z_index;

    m_batch_uv_data[j+6] = glyph.uv_min.x;
    m_batch_uv_data[j+7] = glyph.uv_max.y;

    ++m_num_glyphs_batched;
}

void TextRenderer::render_batch()
{
    const VertexBuffer& vbo = *m_vbo_ring[m_ring_index];
    const VertexArray& vao = *m_vao_ring[m_ring_index];

    vbo.bind();
    vao.bind();

    vbo.buffer(m_batch_position_data.data(), m_glyph_layout, 0);
    vbo.buffer(m_batch_uv_data.data(), m_glyph_layout, 1);

    m_ibo->bind();
    
    GL_CALL(
        glDrawElements(
            GL_TRIANGLES,
            (i32)(m_num_glyphs_batched * GLYPH_INDEX_COUNT),
            GL_UNSIGNED_INT,
            nullptr
        )
    );

    m_ibo->unbind();
    vao.unbind();

    m_num_glyphs_batched = 0;

    m_batch_position_data.clear();
    m_batch_uv_data.clear();

    m_ring_index = (m_ring_index + 1) % BUFFER_RING_SIZE;
}

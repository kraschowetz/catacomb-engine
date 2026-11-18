#include <cat/gfx/text_renderer.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <memory>

#include "cat/core/components/c_transform.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include <cat/gfx/gfx_util.hpp>
#include "cat/gfx/glyph_quad.hpp"
#include "cat/gfx/vertex_buffer.hpp"

using namespace cat;

TextRenderer::TextRenderer()
{
    ASSERT(
        GfxEngine::is_loaded(),
        "gfx engine is not loaded; avoid user-owned TextRenderer's"
    );

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
    Shared<Font> font = text.get_font();
    if(
        m_num_glyphs_batched + text.get_content().size() >= GLYPH_BATCH_SIZE    ||
        text.get_color() != m_current_ink_color                                 ||
        font != m_current_font
    )
    {
        render_batch();
    }

    if(font != m_current_font)
    {
        GfxEngine::get().get_current_render_context()->set_texture(
            font->get_atlas()
        );
        GfxEngine::get().get_current_render_context()->set_font_pixel_range(
            font->get_pixel_range()
        );

        m_current_font = font;
    }

    if(text.get_color() != m_current_ink_color)
    {
        GfxEngine::get().get_current_render_context()->set_modulate_color(
            text.get_color()
        );
        m_current_ink_color = text.get_color();
    }

    std::vector<GlyphQuad> layout = layout_text(
        text.get_content(),
        *text.get_font(),
        text.get_font_size()
    );

    u64 lenght = static_cast<u64>(
        static_cast<f32>(text.get_content().size()) * text.get_visible_ratio()
    );

    for(u64 i = 0; i < lenght; ++i)
    {
        add_glyph_to_batch(layout[i], transform);
    }
}

void TextRenderer::add_glyph_to_batch(const GlyphQuad& glyph, const cTransform& transform)
{
    constexpr u64 NUM_POSITIONS = 12;
    constexpr u64 NUM_UVS = 8;

    u64 i = m_batch_position_data.size();
    u64 j = m_batch_uv_data.size();

    m_batch_position_data.resize(i + NUM_POSITIONS);
    m_batch_uv_data.resize(j + NUM_UVS);

    const glm::mat4 model = transform.as_mat4();

    const f32 left      = glyph.position.x;
    const f32 right     = glyph.position.x + glyph.size.x;
    const f32 bottom    = glyph.position.y;
    const f32 top       = glyph.position.y + glyph.size.y;

    const glm::vec3 local_corners[4] = {
        {left, bottom, 0.f},
        {right, bottom, 0.f},
        {right, top, 0.f},
        {left, top, 0.f}
    };

    for(u64 corner = 0; corner < 4; ++corner)
    {
        glm::vec3 world = glm::vec3{model * glm::vec4{local_corners[corner], 1.f}};

        m_batch_position_data[i + corner*3 + 0] = world.x;
        m_batch_position_data[i + corner*3 + 1] = world.y;
        m_batch_position_data[i + corner*3 + 2] = world.z;
    }


    // bottom left
    m_batch_uv_data[j] = glyph.uv_min.x;
    m_batch_uv_data[j+1] = glyph.uv_min.y;

    // bottom right
    m_batch_uv_data[j+2] = glyph.uv_max.x;
    m_batch_uv_data[j+3] = glyph.uv_min.y;

    // top right
    m_batch_uv_data[j+4] = glyph.uv_max.x;
    m_batch_uv_data[j+5] = glyph.uv_max.y;

    // top left
    m_batch_uv_data[j+6] = glyph.uv_min.x;
    m_batch_uv_data[j+7] = glyph.uv_max.y;

    ++m_num_glyphs_batched;
}

void TextRenderer::render_batch()
{
    Watcher<RenderContext> ctx = GfxEngine::get().get_current_render_context();

    if(ctx->is_dirty())
        ctx->update();

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

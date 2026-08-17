#pragma once

#include <cat/core/components/c_world_transform.hpp>
#include <cat/core/components/c_transform.hpp>

#include <cat/util/util.hpp>
#include <cat/util/memory.hpp>

#include <cat/gfx/vertex_buffer.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/vertex_layout.hpp>
#include <cat/gfx/glyph_quad.hpp>
#include <cat/gfx/components/c_text.hpp>

namespace cat
{

class TextRenderer
{
public:
    TextRenderer();

    NO_COPY(TextRenderer);

    void render_text(const cText& text, const cTransform& transform);
    void render_text(const cText& text, const cWorldTransform& transform);

    bool has_glyphs_batched() const;

private:
    void add_glyph_to_batch(
        const GlyphQuad& glyph,
        const glm::vec3& position
    );

    void render_batch();

private:
    static constexpr u64 BUFFER_RING_SIZE = 3;
    static constexpr u64 GLYPH_BATCH_SIZE = 64;
    static constexpr u32 GLYPH_VERTEX_COUNT = 4;
    static constexpr u32 GLYPH_INDEX_COUNT = 6;

    static constexpr u64 GLYPH_VERTEX_SIZE = 
        (sizeof(f32) * 3) +     // position
        (sizeof(f32) * 2)       // uv
    ;

    std::array<Unique<VertexBuffer>, BUFFER_RING_SIZE> m_vbo_ring;
    std::array<Unique<VertexArray>, BUFFER_RING_SIZE> m_vao_ring;

    Unique<VertexBuffer> m_ibo;
    VertexLayout m_glyph_layout;

    std::vector<f32> m_batch_position_data;
    std::vector<f32> m_batch_uv_data;

    glm::vec4 m_current_ink_color;
    u32 m_current_font_handle;
    u32 m_num_glyphs_batched = 0;
    u32 m_ring_index = 0;

    friend class GfxEngine;
};

}

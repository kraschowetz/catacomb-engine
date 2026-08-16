#pragma once

#include "cat/gfx/texture.hpp"
#include "cat/util/memory.hpp"
#include "cat/util/util.hpp"
#include <cat/config.hpp>
#include <cat/util/math.hpp>

namespace cat
{

struct GlyphInfo
{
    glm::vec2 uv_min, uv_max;
    glm::vec2 plane_min, plane_max;
    f32 advance;
    bool has_ink;
};

class Font
{
public:
    Font()
        : m_pixel_range(4.0f)
        , m_line_height(1.2f)
        , m_ascender(0.f)
        , m_descender(0.f)
    {}

    Font(Font&&) noexcept = default;
    Font& operator=(Font&&) noexcept = default;

    NO_COPY(Font);

    const Watcher<GlyphInfo> get_glyph(u32 codepoint) const;

    const Shared<Texture> get_atlas() const;
    f32 get_pixel_range() const;
    f32 get_line_height() const;
    f32 get_ascender() const;
    f32 get_descenfer() const;

private:
    std::unordered_map<u32, GlyphInfo> m_glyphs;
    Texture m_atlas;
    f32 m_pixel_range;
    f32 m_line_height;
    f32 m_ascender;
    f32 m_descender;

    friend struct FontLoader;
};

}

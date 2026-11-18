#include <cat/gfx/font.hpp>

using namespace cat;

Watcher<const GlyphInfo> Font::get_glyph(u32 codepoint) const
{
    ASSERT(m_glyphs.contains(codepoint), "font dont have character {}", codepoint);

    return &m_glyphs.at(codepoint);
}

Watcher<Texture> Font::get_atlas()
{
    return &m_atlas;
}

f32 Font::get_pixel_range() const { return m_pixel_range; }
f32 Font::get_line_height() const { return m_line_height; }
f32 Font::get_ascender() const { return m_ascender; }
f32 Font::get_descender() const { return m_descender; }

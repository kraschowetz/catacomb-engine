#include <cat/gfx/glyph_quad.hpp>

#include "cat/gfx/font.hpp"

namespace cat
{

std::vector<GlyphQuad> layout_text(
    const std::string& content,
    const Font& font,
    f32 font_size
)
{
    std::vector<GlyphQuad> quads;
    quads.reserve(content.size());
    
    glm::vec2 cursor{0.f};

    for(char c : content)
    {
        if(c == '\n')
        {
            cursor.x = 0.f;
            cursor.y -= font.get_line_height() * font_size;
            continue;
        }

        const Watcher<const GlyphInfo> glyph = font.get_glyph(static_cast<u8>(c));
        if(!glyph) continue;

        if(glyph->has_ink)
        {
            quads.push_back(
                GlyphQuad {
                    cursor + glyph->plane_min * font_size,
                    (glyph->plane_max - glyph->plane_min) * font_size,
                    glyph->uv_min,
                    glyph->uv_max
                }
            );

        }

        cursor.x += glyph->advance * font_size;
    }

    return quads;
}

}

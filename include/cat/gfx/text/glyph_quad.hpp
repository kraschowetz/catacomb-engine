#pragma once

#include "cat/gfx/text/font.hpp"
#include <cat/util/math.hpp>
#include <string>
#include <vector>

namespace cat
{

struct GlyphQuad
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 uv_min;
    glm::vec2 uv_max;
};

std::vector<GlyphQuad> layout_text(
    const std::string& content,
    const Font& font,
    f32 font_size
);
}

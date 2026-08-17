#include <cat/gfx/components/c_text.hpp>

#include <glm/fwd.hpp>
#include <string>

using namespace cat;

glm::vec4 cText::get_color() const { return m_color; }
f32 cText::get_font_size() const { return m_font_size; }
const std::string& cText::get_content() const { return m_content; }
const Shared<Font> cText::get_font() const { return m_font; }

void cText::set_content(const std::string& content)
{
    m_content = content;
    m_dirty = true;
}

void cText::set_font(Shared<Font>& font)
{
    m_font.swap(font);
    m_dirty = true;
}

void cText::set_font_size(f32 size)
{
    m_font_size = size;
    m_dirty = true;
}

void cText::set_color(const glm::vec4& color)
{
    m_color = color;
}

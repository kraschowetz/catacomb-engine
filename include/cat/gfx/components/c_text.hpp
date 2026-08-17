#pragma once

#include "cat/gfx/font.hpp"
#include "cat/util/memory.hpp"
#include <cat/config.hpp>

#include <cat/core/glz_meta.hpp>

namespace cat
{

struct cText
{
public:
    cText() = default;
    cText(const std::string& content, const Shared<Font>& font)
        : m_content(content)
        , m_font(font)
    {}

    glm::vec4 get_color() const;
    f32 get_font_size() const;
    const std::string& get_content() const;
    const Shared<Font> get_font() const;


    void set_content(const std::string& content);
    void set_font(Shared<Font>& font);
    void set_font_size(f32 size);
    void set_color(const glm::vec4& color);

private:
    std::string m_content;
    Shared<Font> m_font;
    glm::vec4 m_color = glm::vec4{1.f};
    f32 m_font_size   = 32.f;
    bool m_dirty      = true;

    friend class TextRenderer;
};

}

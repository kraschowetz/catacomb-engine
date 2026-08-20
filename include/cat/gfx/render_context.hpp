#pragma once

#include "cat/gfx/csl/csl.hpp"
#include "cat/gfx/shader.hpp"
#include <cat/util/memory.hpp>
#include <cat/util/math.hpp>

namespace cat
{

struct RenderContext
{
public:
    RenderContext() = default;
    RenderContext(const Watcher<Shader> shader)
        : m_view{1.f}
        , m_projection{1.f}
        , m_modulate_color{1.f}
        , m_default_shader(shader)
    {
        m_textures.fill(nullptr);
    }

    bool is_dirty() const;

    void set_view(const glm::mat4& mat);
    void set_projection(const glm::mat4& mat);
    void set_modulate_color(const glm::vec4& color);
    void set_font_pixel_range(f32 range);
    void set_texture(Watcher<Texture> texture, u32 index = 0);

    glm::mat4 get_view() const { return m_view; }
    glm::mat4 get_projection() const { return m_projection; }
    glm::vec4 get_modulate_color() const { return m_modulate_color; }
    f32 get_font_pixel_range() const { return m_font_pixel_range; }

    // @brief apply this context's info to a shader
    // @param shader: which shader to bind, defaults to `default_shader`
    void bind(Shared<Shader> shader = nullptr);
    void update(Shared<Shader> shader = nullptr);

private:
    glm::mat4 m_view;
    glm::mat4 m_projection;

    glm::vec4 m_modulate_color;

    Watcher<Shader> m_default_shader;

    std::array<Watcher<Texture>, CAT_CSL_NUM_TEXTURE_SLOTS> m_textures;

    f32 m_font_pixel_range;

    bool m_dirty;

    // TODO: FrameBuffer* output;
};

}

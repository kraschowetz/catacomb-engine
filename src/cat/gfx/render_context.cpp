#include "cat/gfx/csl/csl.hpp"
#include <cat/gfx/render_context.hpp>

using namespace cat;

void RenderContext::bind(Shared<Shader> arg_shader) const
{
    const Shader& shader = arg_shader == nullptr ? *m_default_shader : *arg_shader;

    shader.bind();
}

void RenderContext::update(Shared<Shader> arg_shader)
{
    const Shader& shader = arg_shader == nullptr ? *m_default_shader : *arg_shader;

    shader.set_model_matrix(glm::mat4{1.f});
    shader.set_view_matrix(m_view);
    shader.set_projection_matrix(m_projection);
    shader.set_modulate_color(m_modulate_color);
    shader.set_font_pixel_range(m_font_pixel_range);

    for(u32 i = 0; i < CAT_CSL_NUM_TEXTURE_SLOTS; ++i)
    {
        if(m_textures[i]){
            m_textures[i]->bind(i);
            shader.set_texture(*m_textures[i], i);
        }
    }

    m_dirty = false;
}

bool RenderContext::is_dirty() const
{
    return m_dirty;
}

void RenderContext::set_view(const glm::mat4& mat)
{
    m_view = mat;
    m_dirty = true;
}

void RenderContext::set_projection(const glm::mat4& mat)
{
    m_projection = mat;
    m_dirty = true;
}

void RenderContext::set_modulate_color(const glm::vec4& color)
{
    m_modulate_color = color;
    m_dirty = true;
}

void RenderContext::set_font_pixel_range(f32 range)
{
    m_font_pixel_range = range;
    m_dirty = true;
}

void RenderContext::set_texture(Watcher<Texture> texture, u32 id)
{
    m_textures[id] = texture;
    m_dirty = true;
}

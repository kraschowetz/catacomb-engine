#include <cat/gfx/shader.hpp>

#include <SDL2/SDL.h>

#include <cat/gfx/gfx_util.hpp>

using namespace cat;

Shader::Shader(u32 handle) : m_handle(handle) {}
Shader::Shader() : m_handle(0) {}

void Shader::bind() const
{
    GL_CALL(glUseProgram(m_handle));
}

void Shader::unbind() const
{
    GL_CALL(glUseProgram(0));
}

u32 Shader::get_handle() const
{
    return m_handle;
}

u32& Shader::get_handle_ref()
{
    return m_handle;
}

i32 Shader::get_uniform_location(const std::string& name) const
{
    hash_t hash = std::hash<std::string>{}(name);
    return m_uniform_cache.user_uniforms.at(hash);
}

void Shader::set_model_matrix(const glm::mat4& val) const
{
    i32 location = m_uniform_cache.model_matrix;
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]));
}

void Shader::set_view_matrix(const glm::mat4& val) const
{
    i32 location = m_uniform_cache.view_matrix;
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]));
}

void Shader::set_projection_matrix(const glm::mat4& val) const
{
    i32 location = m_uniform_cache.projection_matrix;
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]));
}

void Shader::set_uniform(const std::string& name, f32 val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniform1f(location, val));
}

void Shader::set_uniform(const std::string& name, i32 val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniform1i(location, val));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]));
}

void Shader::set_uniform(const std::string& name, const glm::mat3& val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, &val[0][0]));
}

void Shader::set_uniform(const std::string& name, const glm::vec4& val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniform4fv(location, 1, &val[0]));
}

void Shader::set_uniform(const std::string& name, const glm::vec3& val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniform3fv(location, 1, &val[0]));
}

void Shader::set_uniform(const std::string& name, const glm::vec2& val) const
{
    i32 location = get_uniform_location(name);
    GL_CALL(glUniform2fv(location, 1, &val[0]));
}

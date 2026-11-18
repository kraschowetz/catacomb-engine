#include <cat/gfx/texture.hpp>

#include <cat/gfx/gfx_util.hpp>

using namespace cat;

Texture::Texture(Texture&& other) noexcept
{
    this->m_handle = other.m_handle;
    this->m_target = other.m_target;
    this->m_size = other.m_size;

    other.m_handle = 0;
    other.m_target = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    this->m_handle = other.m_handle;
    this->m_target = other.m_target;
    this->m_size = other.m_size;

    other.m_handle = 0;
    other.m_target = 0;

    return *this;
}

u32& Texture::get_handle_ref()
{
    return m_handle;
}

u32 Texture::get_handle() const
{
    return m_handle;
}

const glm::ivec2& Texture::get_size() const
{
    return m_size;
}

void Texture::bind(u32 unit) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    GL_CALL(glBindTexture(m_target, m_handle));
}

void Texture::unbind(u32 unit) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    GL_CALL(glBindTexture(m_target, 0));
}

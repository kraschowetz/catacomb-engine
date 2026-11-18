#pragma once

#include <cat/config.hpp>
#include <cat/util/util.hpp>

namespace cat
{

struct TextureUV
{
    f32 right_x, left_x, top_y, bottom_y;
};

class Texture
{
public:
    Texture() : m_handle(0), m_target(0) {}
    Texture(const glm::ivec2& size, u32 handle, u32 target)
        : m_size(size)
        , m_handle(handle)
        , m_target(target)
    {}

    NO_COPY(Texture);

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    u32& get_handle_ref();
    u32 get_handle() const;

    const glm::ivec2& get_size() const;
    
    void bind(u32 unit = 0) const;
    void unbind(u32 unit = 0) const;

private:
    glm::ivec2 m_size;

    u32 m_handle;
    u32 m_target;

    friend struct TextureLoader;
};

}

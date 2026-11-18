#pragma once

#include <cat/gfx/vertex_buffer.hpp>

namespace cat
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    NO_COPY(VertexArray);

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind() const;
    void unbind() const;

    void attr(const VertexBuffer& vbo, const VertexLayout& layout) const;

private:
    u32 m_handle;
};

}

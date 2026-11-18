#include <cat/gfx/vertex_array.hpp>

#include "cat/gfx/gfx_util.hpp"
#include "cat/gfx/vertex_layout.hpp"

using namespace cat;

VertexArray::VertexArray()
{
    GL_CALL(glGenVertexArrays(1, &m_handle));
}

VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_handle));
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
    this-> m_handle = other.m_handle;
    other.m_handle = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    this-> m_handle = other.m_handle;
    other.m_handle = 0;
    return *this;
}

void VertexArray::bind() const
{
    GL_CALL(glBindVertexArray(m_handle));
}

void VertexArray::unbind() const
{
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::attr(const VertexBuffer &vbo, const VertexLayout &layout) const
{
    const std::vector<VertexElement>& elements = layout.get_elements();
    u32 offset = 0;
    
    for(u32 i = 0; i < elements.size(); ++i)
    {
        const VertexElement& element = elements[i];

        GL_CALL(glEnableVertexAttribArray(i));
        
        if(element.is_normalized || element.type == GL_FLOAT || element.type == GL_DOUBLE)
        {
            GL_CALL(
                glVertexAttribPointer(
                    i,
                    (i32)element.count,
                    element.type,
                    element.is_normalized,
                    (i32)(element.get_size() * element.count),
                    (void*)((u64)(offset))
                )
            );
        }
        else
        {
            GL_CALL(
                glVertexAttribIPointer(
                    i,
                    (i32)element.count,
                    element.type,
                    (i32)(element.get_size() * element.count),
                    (void*)((u64)(offset))
                )
            );
        }

        offset += element.get_size() * element.count * vbo.m_vertex_count;
    }
}

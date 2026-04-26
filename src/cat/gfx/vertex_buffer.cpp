#include "cat/gfx/vertex_layout.hpp"
#include <cat/gfx/vertex_buffer.hpp>

#include <cat/gfx/gfx_util.hpp>

using namespace cat;

static constexpr u32 NULL_HANDLE = 0;

VertexBuffer::VertexBuffer(u32 vertex_size, u32 vertex_count, eBufferType type)
{
	m_vertex_count = vertex_count;

	switch(type)
	{
		case eBufferType::INDEX:
			m_target = GL_ELEMENT_ARRAY_BUFFER;
			break;
		case eBufferType::VERTEX:
			m_target = GL_ARRAY_BUFFER;
			break;
	}

	GL_CALL(glGenBuffers(1, &m_handle));

	bind();

	// allocate memory to buffer
	GL_CALL(
		glBufferData(
			m_target,
			vertex_count * vertex_size,
			NULL,
			GL_STATIC_DRAW
		);
	);

	unbind();
}

VertexBuffer::~VertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &m_handle));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
	this->m_handle = other.m_handle;
	this->m_target = other.m_target;
	this->m_vertex_count = other.m_vertex_count;
	
	other.m_handle = 0;
	other.m_vertex_count = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	this->m_handle = other.m_handle;
	this->m_target = other.m_target;
	this->m_vertex_count = other.m_vertex_count;
	
	other.m_handle = NULL_HANDLE;
	other.m_vertex_count = 0;

	return *this;
}	

void VertexBuffer::bind() const
{
	GL_CALL(glBindBuffer(m_target, m_handle));
}

void VertexBuffer::unbind() const
{
	GL_CALL(glBindBuffer(m_target, NULL_HANDLE));
}

void VertexBuffer::buffer(const void* data, VertexLayout& layout, u32 index) const
{
	bind();
	
	u32 offset = layout.get_offset(index);
	const VertexElement& element = layout.get_element(index);

	if(m_target != GL_ELEMENT_ARRAY_BUFFER) 
	{
		GL_CALL(
			glBufferSubData(
				m_target,
				offset * m_vertex_count,
				element.get_size() * element.count * m_vertex_count,
				data
			)
		);
	}

	unbind();
}

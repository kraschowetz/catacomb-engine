#pragma once

#include <cat/gfx/vertex_layout.hpp>
#include <cat/util/util.hpp>

namespace cat
{

enum class eBufferType : u8
{
	VERTEX,
	INDEX
};

class VertexBuffer
{
public:
	// @brief: setup and allocate memory for a gl vertex buffer
	// @param 1: sizeof each vertex, in bytes
	// @param 2: number of vertices in the buffer
	// @param 3: the buffer type
	VertexBuffer(u32 vertex_size, u32 vertex_count, eBufferType type);
	~VertexBuffer();

	NO_COPY(VertexBuffer);

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void bind() const;
	void unbind() const;

	// @brief: feeds data into the buffer
	// @param 1: pointer to src data
	// @param 2: VertexLayout describinf the data
	// @param 3: vertex attribute to receive the data
	void buffer(const void* data, VertexLayout& layout, u32 index) const;

private:
	u32 m_handle;
	u32 m_target;
	u32 m_vertex_count;

	friend class VertexArray;
};

}

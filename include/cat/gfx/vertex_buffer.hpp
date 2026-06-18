#pragma once

#include <cat/gfx/vertex_layout.hpp>    // IWYU pragma: export
#include <cat/util/util.hpp>            // IWYU pragma: export

namespace cat
{

enum class eBufferType : u8
{
	VERTEX,
	INDEX
};

enum class eBufferUsage : u8
{
    STATIC,
    DYNAMIC,
    STREAM
};

class VertexBuffer
{
public:
	// brief: setup and allocate memory for a gl vertex buffer
	// param 1: sizeof each vertex, in bytes
	// param 2: number of vertices in the buffer
	// param 3: the buffer type
	// param 4: the buffer usage (where the memory will be allocated in GPU)
	VertexBuffer(
        u32 vertex_size, u32 vertex_count, 
        eBufferType type, eBufferUsage usage = eBufferUsage::STATIC
    );
	~VertexBuffer();

	NO_COPY(VertexBuffer);

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void bind() const;
	void unbind() const;
    void orphan() const;

	// brief: feeds data into the buffer
	// param 1: pointer to src data
	// param 2: VertexLayout describinf the data
	// param 3: vertex attribute to receive the data
	void buffer(const void* data, VertexLayout& layout, u32 index) const;

    void upload_indices(const u32* src, u64 num_indices) const;

private:
	u32 m_handle;
	u32 m_target;
	u32 m_vertex_count;
    u32 m_vertex_size;

	friend class VertexArray;
};

}

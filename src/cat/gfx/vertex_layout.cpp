#include <cat/gfx/vertex_layout.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <vector>

using namespace cat;

u32 VertexElement::get_size() const
{
	switch(type)
	{
		case GL_DOUBLE:		return sizeof(f64);
		case GL_FLOAT:		return sizeof(f32);
		case GL_UNSIGNED_INT: 	return sizeof(u32);
		case GL_UNSIGNED_SHORT:	return sizeof(u16);
		case GL_UNSIGNED_BYTE: 	return sizeof(u8);
		case GL_INT: 		return sizeof(i32);
		case GL_SHORT:	 	return sizeof(i16);
		case GL_BYTE: 		return sizeof(i8);
		default: 		return 0;
	}	
}

u32 VertexLayout::get_stride() const
{
	return m_stride;
}

const std::vector<VertexElement>& VertexLayout::get_elements() const
{
	return m_elements;
}

const VertexElement& VertexLayout::get_element(u32 index) const
{
	return m_elements[index];
}

u32 VertexLayout::get_offset(u32 index) const
{
	if(index == 0) return 0;

	CAT_ASSERT(index < m_elements.size());
	
	u32 offset = 0;
	for(u32 i = 0; i < index; ++i)
	{
		const VertexElement& element = m_elements[i];

		offset += element.count + element.get_size();
	}

	return offset;
}

void VertexLayout::push_f64(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_DOUBLE, count, normalized);
	m_stride += sizeof(f64) * count;
}

void VertexLayout::push_f32(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_FLOAT, count, normalized);
	m_stride += sizeof(f32) * count;
}

void VertexLayout::push_u32(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_UNSIGNED_INT, count, normalized);
	m_stride += sizeof(u32) * count;
}

void VertexLayout::push_u16(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_UNSIGNED_SHORT, count, normalized);
	m_stride += sizeof(u16) * count;
}

void VertexLayout::push_u8(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_UNSIGNED_BYTE, count, normalized);
	m_stride += sizeof(u8) * count;
}

void VertexLayout::push_i32(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_INT, count, normalized);
	m_stride += sizeof(i32) * count;
}

void VertexLayout::push_i16(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_SHORT, count, normalized);
	m_stride += sizeof(i16) * count;
}

void VertexLayout::push_i8(u32 count, u8 normalized)
{
	m_elements.emplace_back(GL_BYTE, count, normalized);
	m_stride += sizeof(i8) * count;
}

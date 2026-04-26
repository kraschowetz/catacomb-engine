#pragma once

#include <cat/config.hpp>
#include <vector>

namespace cat
{

struct VertexElement
{
	u32  type;
	u32  count;
	bool is_normalized;

	VertexElement(u32 _type, u32 _count, u8 _normalized)
		: type(_type)
		, count(_count)
		, is_normalized(_normalized)
	{}

	u32 get_size() const;
};

struct VertexLayout
{
public:
	VertexLayout() : m_stride(0) {};
	~VertexLayout() = default;

	u32 get_stride() const;
	const std::vector<VertexElement>& get_elements() const;
	const VertexElement& get_element(u32 index) const;
	u32 get_offset(u32 index) const;

	void push_f64(u32 count, u8 normalized = false);
	void push_f32(u32 count, u8 normalized = false);
	void push_u32(u32 count, u8 normalized = false);
	void push_u16(u32 count, u8 normalized = false);
	void push_u8(u32 count, u8 normalized = false);
	void push_i32(u32 count, u8 normalized = false);
	void push_i16(u32 count, u8 normalized = false);
	void push_i8(u32 count, u8 normalized = false);

private:
	std::vector<VertexElement> m_elements;
	u32 m_stride;
};

}

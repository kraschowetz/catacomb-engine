#ifndef VAO_H
#define VAO_H

#include "vbo.hpp"

class VAO {
public:
	u32 handle;

	void init();
	void destroy();
	void bind();
	void attr(VBO& vbo, u32 index, i32 size, u32 type, i32 stride, u64 offset);
	void disable_attr(u32 index);
};

#endif // VAO_H

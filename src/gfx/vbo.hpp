#ifndef VBO_H
#define VBO_H

#include "gfx_util.hpp"

class VBO {
public:
	u32 handle, type;

	void init(u32 type);
	void destroy();
	void bind();
	void buffer(const void *data, i64 offset, i64 count);
};

#endif // VBO_H

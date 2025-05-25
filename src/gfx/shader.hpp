#ifndef SHADER_H
#define SHADER_H

#include "gfx.hpp"

class Shader {
public:
	u32 handle, vertex, fragment;

	void init(const char *vertex, const char *fragment);
	void destroy();

	void bind();
	void uniform_f32(const char *name, f32 value);
	void uniform_mat4(const char *name, glm::mat4 value);
	void uniform_i32(const char *name, i32 value);
};

#endif // SHADER_H

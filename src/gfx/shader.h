#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"

typedef struct {
	GLuint handle;
	GLuint vertex_shader;
	GLuint fragment_shader;
} Shader;

Shader create_shader(const char *vertex, const char *fragment);
void destroy_shader(Shader *self);
void shader_bind(const Shader *self);
void shader_uniform_float(const Shader *self, const char *name, f32 val);
void shader_uniform_mat4(const Shader *self, const char *name, mat4 m);
void shader_uniform_mat4s(const Shader *self, const char *name, mat4s m);
void shader_uniform_int(const Shader *self, const char *name, i32 val);

#endif

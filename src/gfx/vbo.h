#ifndef VAO_H
#define VAO_H

#include "gfx.h"

typedef struct VBO {
	GLuint handle;
	GLuint type;
} VBO;

VBO create_vbo(GLuint type);
void destroy_vbo(const VBO *self);
void vbo_bind(const VBO *self);
void vbo_buffer(const VBO *self, const void *data, GLsizeiptr offset, GLsizeiptr count);

#endif // VAO_H

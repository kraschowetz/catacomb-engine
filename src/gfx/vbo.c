#include "vbo.h"

VBO create_vbo(GLuint type) {
	VBO self = {.type = type};
	glGenBuffers(1, &self.handle);

	return self;
}

void destroy_vbo(const VBO *self) {
	glDeleteBuffers(1, &self->handle);
}

void vbo_bind(const VBO *self) {
	glBindBuffer(self->type, self->handle);
}

void vbo_buffer(const VBO *self, const void *data, GLsizeiptr offset, GLsizeiptr count) {
	vbo_bind(self);
	glBufferData(self->type, count - offset, data, GL_STATIC_DRAW);
}

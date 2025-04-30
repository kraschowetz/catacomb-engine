#include "vao.h"

VAO create_vao() {
	VAO self;
	glGenVertexArrays(1, &self.handle);
	
	return self;
}

void destroy_vao(VAO *self) {
	glDeleteVertexArrays(1, &self->handle);
}

void vao_bind(const VAO *self) {
	glBindVertexArray(self->handle);
}

void vao_attr(
	const VAO *self, const VBO *vbo, GLuint index, GLint size, 
	GLenum type, GLsizei stride, size_t offset) {
	
	vao_bind(self);
	vbo_bind(vbo);

	glEnableVertexAttribArray(index);

	switch(type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(
				index,
				size,
				type,
				stride,
				(void*)offset
			);
			break;
		default:
			glVertexAttribPointer(
				index,
				size,
				type,
				GL_FALSE,
				stride,
				(void*)offset
			);
			break;
	}
}

void vao_disable_attr(const VAO *self, const GLuint *ids, size_t id_count) {
	vao_bind(self);
	for(u64 i = 0; i < id_count; i++) {
		glDisableVertexAttribArray(ids[i]);
	}
}

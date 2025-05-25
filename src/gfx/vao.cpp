#include "vao.hpp"

void VAO::init() {
	GL_CHECK(glGenVertexArrays(1, &this->handle));
}

void VAO::destroy() {
	glDeleteVertexArrays(1, &this->handle);
}

void VAO::bind() {
	glBindVertexArray(this->handle);
}

void
VAO::attr(
	VBO& vbo, u32 index, i32 size, u32 type, i32 stride, u64 offset
) {
	this->bind();
	vbo.bind();

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

void VAO::disable_attr(u32 index) {
	glDisableVertexAttribArray(index);
}

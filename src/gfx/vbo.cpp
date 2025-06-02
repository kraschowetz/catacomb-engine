#include "vbo.hpp"

void VBO::init(u32 type) {
	this->type = type;
	glGenBuffers(1, &this->handle);
}

void VBO::destroy() {
	glDeleteBuffers(1, &this->handle);
}

void VBO::bind() {
	glBindBuffer(this->type, this->handle);
}

void VBO::buffer(const void *data, i64 offset, i64 count) {
	this->bind();
	glBufferData(this->type, count - offset, data, GL_STATIC_DRAW);
}

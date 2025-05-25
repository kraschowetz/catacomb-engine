#include "triangle.hpp"

Triangle::Triangle(std::vector<f32> position, std::vector<f32> color) {
	ASSERT(position.size() == 9 && color.size() == 9, "invalid position or color vectors");

	this->vao.init();
	this->vbo.init(GL_ARRAY_BUFFER);

	this->vbo.buffer(
		position.data(), 0, sizeof(f32) * 9
	);
	this->vao.attr(
		this->vbo,
		0,
		3,
		GL_FLOAT,
		0,
		0
	);

	this->vbo.init(GL_ARRAY_BUFFER);
	this->vbo.buffer(
		color.data(), 0, sizeof(f32) * 9
	);
	this->vao.attr(
		this->vbo,
		1,
		3,
		GL_FLOAT,
		0,
		0
	);
}

Triangle::~Triangle() {
	this->vbo.destroy();
	this->vao.destroy();
}

void Triangle::render() {
	vao.bind();
	glDrawArrays(
		GL_TRIANGLES,
		0,
		3
	);
}

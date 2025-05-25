#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vao.hpp"
#include <vector>

class Triangle {
public:
	VAO vao;
	VBO vbo;

	Triangle(std::vector<f32> position, std::vector<f32> color);
	~Triangle();

	void render();
};

#endif // TRIANGLE_H

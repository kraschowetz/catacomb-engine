#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/ext/vector_float2.hpp>

struct Transform {
	glm::vec2 position, scale;
	float rotation;
};

#endif // TRANSFORM_H

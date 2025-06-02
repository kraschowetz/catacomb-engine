#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../util/util.hpp"

struct Camera2D {
	glm::mat4 view, projection;
	glm::vec2 position, min, max;
	f32 speed, max_distance;

	Camera2D(glm::vec2 min, glm::vec2 max) :
		position(glm::vec2(0, 0)), min(min), max(max),
		speed(32.f), max_distance(32.f) {};

	Camera2D(glm::vec2 min, glm::vec2 max, f32 speed, f32 max_distance) :
		position(glm::vec2(0, 0)), min(min), max(max),
		speed(speed), max_distance(max_distance) {};
};

#endif // _CAMERA_H

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../util/util.hpp"
#include <limits>

OrthoCamera::OrthoCamera(glm::vec2 min, glm::vec2 max) {
	this->min = min;
	this-> max = max;
	position = glm::vec2(0, 0);

	this->update();
}

void OrthoCamera::update() {
	view_proj.view = glm::mat4(1);
	view_proj.view = glm::translate(
		view_proj.view,
		glm::vec3(-position.x, -position.y, -1.f)
	);
	
	/*
	 * tweaked glm::ortho values so axis are layd out like:
	 * 		-Y
	 * 	-X	  	+X
	 * 		+Y
	*/
	view_proj.projection = glm::ortho(
		min.x,
		max.x,
		max.y,
		min.y,
		(f32)std::numeric_limits<i8>::min() - 1.f,
		(f32)std::numeric_limits<i8>::max() + 1.f
	);
}

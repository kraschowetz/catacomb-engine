#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct ViewProjection {
	glm::mat4 view, projection;
};

class OrthoCamera {
public:
	ViewProjection view_proj;
	glm::vec2 position, min, max;

	OrthoCamera(glm::vec2 min, glm::vec2 max);
	~OrthoCamera() = default;

	void update();
};

#endif // CAMERA_H

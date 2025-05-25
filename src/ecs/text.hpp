#ifndef TEXT_H
#define TEXT_H

#include <glm/glm.hpp>
#include "../util/util.hpp"

struct Text {
	const char *text;
	u16 size;		// font size in pixels
	glm::ivec2 bounds; 	// bounds in characters
};

#endif // TEXT_H

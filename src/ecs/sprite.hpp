#ifndef SPRITE_H
#define SPRITE_H

#include <glm/ext/vector_int2.hpp>
#include "../util/util.hpp"

struct Sprite {
	i8 z_index;
	glm::ivec2 atlas_coords, size;
	u8 atlas_index;
};

#endif // SPRITE_H

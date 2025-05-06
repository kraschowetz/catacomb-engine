#ifndef SPRITE_H
#define SPRITE_H

#include "../util/util.h"
#include <cglm/types-struct.h>

typedef struct C_Sprite {
	i16 z_index;
	ivec2s atlas_coords, size;
} C_Sprite;

#endif // SPRITE_H

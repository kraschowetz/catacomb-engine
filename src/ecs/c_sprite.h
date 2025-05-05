#ifndef SPRITE_H
#define SPRITE_H

#include "../gfx/vao.h"
#include <cglm/types-struct.h>

typedef struct C_Sprite {
	i16 z_index;
	ivec2s atlas_coords, size;
} C_Sprite;

#endif // SPRITE_H

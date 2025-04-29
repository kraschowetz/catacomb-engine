#ifndef SPRITE_H
#define SPRITE_H

#include "../gfx/vao.h"

typedef struct C_Sprite {
	VBO position, uv;
	VAO vao;
	u32 gl_handle;
	const char *sprite_path;	// change this to hold sprite id
} C_Sprite;

#endif // SPRITE_H

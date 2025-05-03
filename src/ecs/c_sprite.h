#ifndef SPRITE_H
#define SPRITE_H

#include "../gfx/vao.h"

typedef struct C_Sprite {
	GLuint gl_handle;
	VAO vao;
	VBO vbo_pos, vbo_uv;
	i16 z_index;
} C_Sprite;

#endif // SPRITE_H

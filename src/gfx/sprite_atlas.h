#ifndef SPRITE_ATLAS_H
#define SPRITE_ATLAS_H

#include "gfx.h"
#include "../util/util.h"
#include <cglm/types-struct.h>

typedef struct {
	ivec2s size, sprite_size;
	u32 gl_handle;
	u8 gl_tex_id;
} SpriteAtlas;

typedef struct {
	vec2s raw[4];
} SpriteUV;

SpriteAtlas create_sprite_atlas(const char *path, u8 id, ivec2s sprite_size);
vec2s sprite_atlas_id_to_uv(const SpriteAtlas *self, ivec2s id);
SpriteUV sprite_atlas_id_to_uvs(const SpriteAtlas *self, ivec2s id);
void sprite_atlas_bind(const SpriteAtlas *self);

#endif // SPRITE_ATLAS_H

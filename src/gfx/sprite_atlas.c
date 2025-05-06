#include "sprite_atlas.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb/stb_image.h"

#include "gfx.h"
#include <cglm/struct/vec2.h>

SpriteAtlas create_sprite_atlas(const char *path, u8 id, ivec2s sprite_size) {
	SpriteAtlas self;
	self.sprite_size = sprite_size;
	self.gl_tex_id = id;
	
	stbi_set_flip_vertically_on_load(true);
	u8 *raw_bytes = stbi_load(path, &self.size.x, &self.size.y, NULL, STBI_rgb_alpha);
	
	assert(raw_bytes);

	glGenTextures(1, &self.gl_handle);
	glBindTexture(GL_TEXTURE_2D, self.gl_handle);
	glActiveTexture(id + GL_TEXTURE0); // hack to convert u8 id to GL_TEXTURE macro
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		self.size.x,
		self.size.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		raw_bytes
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(raw_bytes);

	// convert img pixel size to sprite size
	self.size.x /= sprite_size.x;
	self.size.y /= sprite_size.y;

	return self;
}

vec2s sprite_atlas_id_to_uv(const SpriteAtlas *self, ivec2s id) {
	assert(
		id.x < self->size.x && id.y < self->size.y &&
		id.x >= 0 && id.y >= 0
	);

	vec2s result = (vec2s){{
		(f32) (id.x * self->sprite_size.x),
		(f32) (id.y * self->sprite_size.y),
	}};
	
	result.x /= (f32) (self->size.x * self->sprite_size.x);
	result.y /= (f32) (self->size.y * self->sprite_size.y);

	return result;
}

SpriteUV sprite_atlas_id_to_uvs(const SpriteAtlas *self, ivec2s id) {
	assert(
		id.x < self->size.x && id.y < self->size.y &&
		id.x >= 0 && id.y >= 0
	);
	
	SpriteUV result;
	
	f32 h_id2uv, v_id2uv;
	h_id2uv = ((f32) self->size.x * (f32) self->sprite_size.x);
	v_id2uv = ((f32) self->size.y * (f32) self->sprite_size.y);
	
	//top left
	result.raw[3] = (vec2s) {{
		(f32) (id.x * self->sprite_size.y) / h_id2uv,
		(f32) (id.y * self->sprite_size.y) / v_id2uv,
	}};
	//top right
	result.raw[2] = (vec2s) {{
		(f32) ((id.x+1) * self->sprite_size.y) / h_id2uv,
		(f32) (id.y * self->sprite_size.y) / v_id2uv,
	}};
	//bottom right
	result.raw[1] = (vec2s) {{
		(f32) ((id.x+1) * self->sprite_size.y) / h_id2uv,
		(f32) ((id.y+1) * self->sprite_size.y) / v_id2uv,
	}};
	//bottom left
	result.raw[0] = (vec2s) {{
		(f32) (id.x * self->sprite_size.y) / h_id2uv,
		(f32) ((id.y+1) * self->sprite_size.y) / v_id2uv,
	}};

	return result;
}

void sprite_atlas_bind(const SpriteAtlas *self) {
	glBindTexture(GL_TEXTURE_2D, self->gl_handle);
	glActiveTexture(self->gl_tex_id + GL_TEXTURE0);
}

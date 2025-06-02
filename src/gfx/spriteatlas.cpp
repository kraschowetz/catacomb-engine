#include "spriteatlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb/stb_image.h"

SpriteAtlas::SpriteAtlas(const char *path, u8 id, glm::ivec2 sprite_size) {
	this->sprite_size = sprite_size;
	this->gl_tex_id = id;

	stbi_set_flip_vertically_on_load(true);
	u8 *raw_bytes = stbi_load(
		path,
		&this->size.x,
		&this->size.y,
		NULL,
		STBI_rgb_alpha
	);

	ASSERT(raw_bytes, "unable to alloc " << path << " into memory!");

	glGenTextures(1, &this->gl_handle);
	glActiveTexture(id + GL_TEXTURE0); // hack to convert u8 id to GL_TEXTURE macro
	glBindTexture(GL_TEXTURE_2D, this->gl_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		this->size.x,
		this->size.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		raw_bytes
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(raw_bytes);

	this->size.x /= sprite_size.x;
	this->size.y /= sprite_size.y;
}

SpriteAtlas::~SpriteAtlas() {
	glDeleteTextures(1, &this->gl_handle);
}

SpriteUV SpriteAtlas::atlascoords_to_uv(glm::ivec2 coords) {
	SpriteUV result;
	
	// convert spritesheet coords to uv coords
	coords.y = size.y - coords.y - 1;

	f32 h_coords2uv, v_coords2uv;
	h_coords2uv = ((f32) this->size.x * (f32) this->sprite_size.x);
	v_coords2uv = ((f32) this->size.y * (f32) this->sprite_size.y);
	
	//top left
	result[3] = {
		(f32) (coords.x * this->sprite_size.x) / h_coords2uv,
		(f32) (coords.y * this->sprite_size.y) / v_coords2uv,
	};
	//top right
	result[2] = {
		(f32) ((coords.x+1) * this->sprite_size.x) / h_coords2uv,
		(f32) (coords.y * this->sprite_size.y) / v_coords2uv,
	};
	//bottom right
	result[1] = {
		(f32) ((coords.x+1) * this->sprite_size.x) / h_coords2uv,
		(f32) ((coords.y+1) * this->sprite_size.y) / v_coords2uv,
	};
	//bottom left
	result[0] = {
		(f32) (coords.x * this->sprite_size.x) / h_coords2uv,
		(f32) ((coords.y+1) * this->sprite_size.y) / v_coords2uv,
	};

	return result;
}

void SpriteAtlas::bind() {
	glActiveTexture(this->gl_tex_id + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gl_handle);
}

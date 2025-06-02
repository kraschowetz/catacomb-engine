#ifndef SPRITEATLAS_H
#define SPRITEATLAS_H

#include "gfx_util.hpp"
#include <array>

using SpriteUV = std::array<glm::vec2, 4>;

class SpriteAtlas {
private:
public:
	u32 gl_handle;
	glm::ivec2 size, sprite_size;
	u8 gl_tex_id;

	SpriteAtlas(const char *path, u8 id, glm::ivec2 sprite_size);
	~SpriteAtlas();
	SpriteUV atlascoords_to_uv(glm::ivec2 coords);
	void bind();
};

#endif // SPRITEATLAS_H

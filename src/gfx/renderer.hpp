#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.hpp"
#include "vao.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "spriteatlas.hpp"

#include "../ecs/sprite.hpp"
#include "../ecs/transform.hpp"

#define SPRITES_PER_BATCH 64
#define SPRITE_VBO_SIZE 5
#define SPRITE_VERTICES 4
#define BATCH_RENDER_TRESHOLD \
	((SPRITES_PER_BATCH - 1) * SPRITE_VBO_SIZE * SPRITE_VERTICES)

enum ShaderPass {
	SPRITE_SHADER = 0
};
#define LAST_SHADER SPRITE_SHADER

enum SpriteAtlasType {
	TILESET_ATLAS = 0,
	TEXT_ATLAS
};
#define LAST_SPRITEATLAS TEXT_ATLAS

class Renderer {
private:
	SDL_GLContext context;
	bool vsync;

	struct {
		VAO vao;
		VBO vbo;
		f32 *vbo_data;
		u16 top;
	} sprite_batcher;

	struct {
		glm::mat4 model, rotation, translation, scale;
	} mat;

	union {
		Shader raw[LAST_SHADER+1];
		struct {
			Shader sprite;
		};
	} shaders;

	union {
		SpriteAtlas *raw[LAST_SPRITEATLAS + 1];
		struct {
			SpriteAtlas *tileset;
			SpriteAtlas *text;
		};
	} atlasses;
	i16 last_atlas_used = -1;

	OrthoCamera *camera;

	void batch_render_sprites(u8 atlas_id);
public:
	Renderer();
	~Renderer();

	void prepare();
	void render();

	void add_sprite_to_batch(Sprite& sprite, Transform& transform);
};

#endif // RENDERER_H

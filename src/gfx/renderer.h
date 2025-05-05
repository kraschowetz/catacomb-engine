#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "vao.h"
#include "sprite_atlas.h"

#define SPRITES_PER_BATCH 64
#define SPRITE_VBO_SIZE 5
#define SPRITE_VERTICES 4
#define BATCH_RENDER_TRESHOLD \
	((SPRITES_PER_BATCH - 1) * SPRITE_VBO_SIZE * SPRITE_VERTICES)

typedef enum {
	TEXTURE_SHADER
} ShaderPass;
#define LAST_SHADER  TEXTURE_SHADER

typedef enum {
	TILESET_ATLAS,
	ENTITY_ATLAS
} AtlasType;
#define LAST_ATLAS ENTITY_ATLAS

typedef struct Renderer {
	SDL_GLContext context;
	bool vsync_state;
	f32 near_plane, far_plane;
	OrthoCamera *camera;

	struct {
		VAO vao;
		VBO vbo;
		f32 *vbo_data;
		u16 top;
	} sprite_batcher;

	struct {
		mat4s model, rotation, translation, scale;
	} mat;

	union {
		Shader raw[LAST_SHADER + 1];
		struct {
			Shader texture;
		};
	} shaders;

	union {
		SpriteAtlas raw[LAST_ATLAS + 1];
		struct {
			SpriteAtlas tileset, entity;
		};
	} atlasses;

} Renderer;

Renderer create_renderer(SDL_Window *window);
void destroy_renderer(Renderer *self);
void render_prepare(void);
void render(Renderer *self, SDL_Window *window);

void renderer_add_sprite_to_batch(Renderer *self, void *sprite, void *transform);

#endif // RENDERER_H

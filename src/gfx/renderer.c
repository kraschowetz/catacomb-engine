#include "renderer.h"
#include "../core/state.h"
#include "camera.h"
#include "gfx.h"
#include "../ecs/c_sprite.h"

#include "../../include/stb/stb_image.h"
#include <cglm/struct/mat4.h>

Renderer create_renderer(SDL_Window *window) {
	
	// init render variables
	Renderer self = (Renderer) {
		.vsync_state = 0,
		.far_plane = 10.f,
		.near_plane = 0.1f
	};
	
	// init gl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	self.context = SDL_GL_CreateContext(window);
	if(!self.context) {
		THROW_ERR("failed to init OpenGL\n");
	}

	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		THROW_ERR("failed to load OpenGl\n");
	}

	if(SDL_GL_SetSwapInterval(self.vsync_state) < 0) {
		THROW_ERR("failed to enable vsync\n");
	}
	
	// setup sprite batcher
	self.sprite_batcher.vao = create_vao();
	self.sprite_batcher.vbo = create_vbo(GL_ARRAY_BUFFER);
	self.sprite_batcher.vbo_data = calloc(
		SPRITES_PER_BATCH * SPRITE_VBO_SIZE * SPRITE_VERTICES,
		sizeof(f32)
	);
	
	// setup matrices
	self.mat.model = GLMS_MAT4_IDENTITY;
	self.mat.rotation = GLMS_MAT4_IDENTITY;
	self.mat.translation = GLMS_MAT4_IDENTITY;
	self.mat.scale = GLMS_MAT4_IDENTITY;

	self.shaders.texture = create_shader(
		"./res/shaders/texture.vert",
		"./res/shaders/texture.frag"
	);
	self.camera = create_ortho_camera(
		(vec2s){{0.0f, 0.0f}},
		(vec2s){{800.0f, 600.0f}}
	);

	self.atlasses.tileset = create_sprite_atlas(
		"./res/img.png",
		0,
		(ivec2s){{16, 16}}
	);

#if DEBUG_GL_SPECS 
	printf(
		"vendor:\t\t%s\nrenderer:\t%s\nversion:\t%s\nlang_version:\t%s\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION)
	);
	i32 mtexpsh, mtexpbnd;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mtexpsh);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mtexpbnd);
	printf(
		"max textures per shader: \t%d\nmax textures per bind: \t%d\n",
		mtexpsh,
		mtexpbnd
	);
#endif

	return self;
}

static void _batch_render_sprites(Renderer *self) {
	vbo_buffer(
		&self->sprite_batcher.vbo,
		self->sprite_batcher.vbo_data,
		0,
		sizeof(f32) * SPRITES_PER_BATCH * SPRITE_VBO_SIZE * SPRITE_VERTICES
	);
	vao_attr(
		&self->sprite_batcher.vao,
		&self->sprite_batcher.vbo,
		0,
		3,
		GL_FLOAT,
		5 * sizeof(f32),
		0
	);
	vao_attr(
		&self->sprite_batcher.vao,
		&self->sprite_batcher.vbo,
		1,
		2,
		GL_FLOAT,
		5 * sizeof(f32),
		sizeof(f32) * 3
	);
	
	// prolly make this a constexpr (or whater is its equivalent in C)
	u32 ids[SPRITES_PER_BATCH * 6] = {0};
	for(u32 i = 0; i < SPRITES_PER_BATCH; i++) {
		ids[i*6] = i*4;
		ids[(i*6)+1] = i*4+1;
		ids[(i*6)+2] = i*4+2;
		ids[(i*6)+3] = i*4+2;
		ids[(i*6)+4] = i*4+3;
		ids[(i*6)+5] = i*4;
	}
	
	mat4s _model;
	glm_mat4_mulN(
		(mat4 *[]){
			&self->mat.model.raw,
			&self->mat.translation.raw,
			&self->mat.rotation.raw,
			&self->mat.scale.raw
		},
		4,
		_model.raw
	);

	sprite_atlas_bind(&self->atlasses.tileset);
	shader_bind(&self->shaders.texture);

	shader_uniform_mat4s(
		&self->shaders.texture,
		"u_model",
		_model
	);
	shader_uniform_mat4s(
		&self->shaders.texture,
		"u_perspective",
		self->camera->view_proj.projection
	);
	shader_uniform_mat4s(
		&self->shaders.texture,
		"u_view",
		self->camera->view_proj.view
	);
	shader_uniform_int(
		&self->shaders.texture,
		"u_sprite_atlas",
		self->atlasses.tileset.gl_tex_id
	);

	vao_bind(&self->sprite_batcher.vao);

	glDrawElements(GL_TRIANGLES, SPRITES_PER_BATCH * 6, GL_UNSIGNED_INT, ids);

#if DEBUG_BATCH_VAO
	for(int i = 0; i < SPRITES_PER_BATCH * SPRITE_VERTICES; i++) {
		printf(
			"x: %.2f, y: %.2f, z: %.2f, u:%.2f, v:%.2f\n",
			self->sprite_batcher.vbo_data[(i*5)],
			self->sprite_batcher.vbo_data[(i*5)+1],
			self->sprite_batcher.vbo_data[(i*5)+2],
			self->sprite_batcher.vbo_data[(i*5)+3],
			self->sprite_batcher.vbo_data[(i*5)+4]
		);
	}

	for(int i = 0; i < SPRITES_PER_BATCH; i++) {
		printf(
			"%u %u %u %u %u %u\n",
			ids[i*6+0],
			ids[i*6+1],
			ids[i*6+2],
			ids[i*6+3],
			ids[i*6+4],
			ids[i*6+5]
		);
	}
#endif
	self->sprite_batcher.top = 0;
	memset(
		self->sprite_batcher.vbo_data,
		0,
		sizeof(f32) * SPRITES_PER_BATCH * SPRITE_VERTICES * SPRITE_VBO_SIZE
	);
}

void renderer_add_sprite_to_batch(Renderer *self, void *sprite, void *transform) {
	if(self->sprite_batcher.top > BATCH_RENDER_TRESHOLD) {
		_batch_render_sprites(self);
	}

	C_Sprite spr = *(C_Sprite*) sprite;
	C_Transform trans = *(C_Transform*) transform;
	
	SpriteUV uv = sprite_atlas_id_to_uvs(
		&self->atlasses.tileset,
		spr.atlas_coords
	);
	
	u16 i = self->sprite_batcher.top;
	
	spr.size.x /= 2;
	spr.size.y /= 2;

	f32 vert_pos[12] = {
		trans.position.x - (f32) spr.size.x,
		trans.position.y - (f32) spr.size.y,
		spr.z_index,

		trans.position.x + (f32) spr.size.x,
		trans.position.y - (f32) spr.size.y,
		spr.z_index,

		trans.position.x + (f32) spr.size.x,
		trans.position.y + (f32) spr.size.y,
		spr.z_index,
		
		trans.position.x - (f32) spr.size.x,
		trans.position.y + (f32) spr.size.y,
		spr.z_index,
	};
	
	for(i32 j = 0; j < 4; j++) {
		i32 k = i+(j*SPRITE_VBO_SIZE);
		self->sprite_batcher.vbo_data[k] = vert_pos[j*3];
		self->sprite_batcher.vbo_data[k+1] = vert_pos[(j*3)+1];
		self->sprite_batcher.vbo_data[k+2] = vert_pos[(j*3)+2];
		self->sprite_batcher.vbo_data[k+3] = uv.raw[j].x;
		self->sprite_batcher.vbo_data[k+4] = uv.raw[j].y;
	}

	self->sprite_batcher.top += SPRITE_VBO_SIZE * SPRITE_VERTICES;

}

void renderer_prepare(void) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(
		0, 0,
		game_state.window.width, game_state.window.height
	);
	glClearColor(0.1f, 0.1f, 0.5f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void render(Renderer *self, SDL_Window *window) {
	renderer_prepare();
	
	ecs_event(&game_state.ecs, ECS_RENDER);
	_batch_render_sprites(self);
	
	SDL_GL_SwapWindow(window);

	ortho_camera_update(self->camera);
}


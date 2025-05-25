#include "renderer.hpp"

#include "../core/gamestate.hpp"
#include "spriteatlas.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <glm/detail/qualifier.hpp>
#include "../core/input.hpp"
#include "textrenderer.hpp"

TextRenderer _text_renderer;

Renderer::Renderer() {
	vsync = 0;
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	context = SDL_GL_CreateContext(gamestate::window->get_sdl_window());
	ASSERT(context, "failed to init SDL context!");

	ASSERT(gladLoadGLLoader(SDL_GL_GetProcAddress), "failed to load OpenGL!")

	ASSERT(SDL_GL_SetSwapInterval(this->vsync) >= 0, "failed to enable vsync!");

	sprite_batcher.vao.init();
	sprite_batcher.vbo.init(GL_ARRAY_BUFFER);
	sprite_batcher.vbo_data = (f32*) calloc(
		SPRITES_PER_BATCH * SPRITE_VBO_SIZE * SPRITE_VERTICES,
		sizeof(f32)
	);

	mat.model = glm::mat4(1);
	mat.rotation = glm::mat4(1);
	mat.translation = glm::mat4(1);
	mat.scale = glm::mat4(1);

	shaders.sprite.init(
		"./res/shaders/texture.vert",
		"./res/shaders/texture.frag"
	);
	
	// tweak this
	camera = new OrthoCamera(glm::vec2(0, 0), glm::vec2(800, 600));

	atlasses.tileset = new SpriteAtlas("./res/img.png", 0, {16, 16});
	atlasses.text = new SpriteAtlas("./res/alphabet.png", 1, {8, 8});

// update to use c++ stdlib
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
}

static constexpr std::array<u32, SPRITES_PER_BATCH * 6> _gen_sprite_batch_ids() {
	std::array<u32, SPRITES_PER_BATCH * 6> ids;
	for(u32 i = 0; i < SPRITES_PER_BATCH; i++) {
		ids[i*6] = i*4;
		ids[(i*6)+1] = i*4+1;
		ids[(i*6)+2] = i*4+2;
		ids[(i*6)+3] = i*4+2;
		ids[(i*6)+4] = i*4+3;
		ids[(i*6)+5] = i*4;
	}

	return ids;
}

void Renderer::batch_render_sprites(u8 atlas_id) {
	sprite_batcher.vbo.buffer(
		sprite_batcher.vbo_data,
		0,
		sizeof(f32) * SPRITES_PER_BATCH * SPRITE_VBO_SIZE * SPRITE_VERTICES
	);

	sprite_batcher.vao.attr(
		sprite_batcher.vbo,
		0,
		3,
		GL_FLOAT,
		sizeof(f32) * 5,
		0
	);
	sprite_batcher.vao.attr(
		sprite_batcher.vbo,
		1,
		2,
		GL_FLOAT,
		sizeof(f32) * 5,
		sizeof(f32) * 3
	);
	
	auto ids = _gen_sprite_batch_ids();

	glm::mat4 _model = mat.model * mat.translation * mat.rotation * mat.scale;

	shaders.sprite.bind();

	shaders.sprite.uniform_mat4("u_model", _model);
	shaders.sprite.uniform_mat4("u_perspective", camera->view_proj.projection);
	shaders.sprite.uniform_mat4("u_view", camera->view_proj.view);
	shaders.sprite.uniform_i32(
		"u_sprite_atlas",
		(i32)atlasses.raw[last_atlas_used]->gl_tex_id
	);
	
	sprite_batcher.vao.bind();

	glDrawElements(GL_TRIANGLES, SPRITES_PER_BATCH * 6, GL_UNSIGNED_INT, ids.data());

#if DEBUG_BATCH_VAO
	for(int i = 0; i < SPRITES_PER_BATCH * SPRITE_VERTICES; i++) {
		std::cout << "x: " << sprite_batcher.vbo_data[(i*5)] << ", "
			<< "y: " << sprite_batcher.vbo_data[(i*5)+1] << ", "
			<< "z: " << sprite_batcher.vbo_data[(i*5)+2] << ", "
			<< "u: " << sprite_batcher.vbo_data[(i*5)+3] << ", "
			<< "v: " << sprite_batcher.vbo_data[(i*5)+4] << "\n"
		;
	}

	for(int i = 0; i < SPRITES_PER_BATCH; i++) {
		std::cout << ids[i*6+0] << " "
			<< ids[i*6+1] << " "
			<< ids[i*6+2] << " "
			<< ids[i*6+3] << " "
			<< ids[i*6+4] << " "
			<< ids[i*6+5] << "\n"
		;
	}
#endif
	sprite_batcher.top = 0;
	memset(
		sprite_batcher.vbo_data,
		0,
		sizeof(f32) * SPRITE_VERTICES * SPRITE_VBO_SIZE * SPRITES_PER_BATCH
	);
}

void Renderer::add_sprite_to_batch(Sprite &sprite, Transform& transform) {
	if(last_atlas_used != sprite.atlas_index) {
		if(last_atlas_used >= 0)
			batch_render_sprites((u8)last_atlas_used);
		last_atlas_used = sprite.atlas_index;
	}
	if(sprite_batcher.top > BATCH_RENDER_TRESHOLD) {
		batch_render_sprites((u8)last_atlas_used);
	}

	SpriteUV uv = atlasses.raw[last_atlas_used]->atlascoords_to_uv(sprite.atlas_coords);

	u16 i = sprite_batcher.top;

	glm::ivec2 _size = glm::ivec2(
		(i32)((f32)sprite.size.x * transform.scale.x), 
		(i32)((f32)sprite.size.y * transform.scale.y)
	) / 2;

	f32 vert_pos[12] = {
		transform.position.x - (f32) _size.x,
		transform.position.y - (f32) _size.y,
		(f32) sprite.z_index,

		transform.position.x + (f32) _size.x,
		transform.position.y - (f32) _size.y,
		(f32) sprite.z_index,

		transform.position.x + (f32) _size.x,
		transform.position.y + (f32) _size.y,
		(f32) sprite.z_index,

		transform.position.x - (f32) _size.x,
		transform.position.y + (f32) _size.y,
		(f32) sprite.z_index
	};

	for(i32 j = 0; j < 4; j++) {
		i32 k =i+(j*SPRITE_VBO_SIZE);
		sprite_batcher.vbo_data[k] = vert_pos[j*3];
		sprite_batcher.vbo_data[k+1] = vert_pos[(j*3)+1];
		sprite_batcher.vbo_data[k+2] = vert_pos[(j*3)+2];
		sprite_batcher.vbo_data[k+3] = uv[(u8)j].x;
		sprite_batcher.vbo_data[k+4] = uv[(u8)j].y;
	}

	sprite_batcher.top += SPRITE_VBO_SIZE * SPRITE_VERTICES;
}


void Renderer::prepare() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(
		0, 0,
		gamestate::window->width, gamestate::window->height
	);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::render() {
	prepare();
	
	auto view = gamestate::ecs->view<Sprite, Transform>();

	view.foreach([&](EntityID id, Sprite& sprite, Transform& transform){
		add_sprite_to_batch(sprite, transform);
		transform.position.x += gamestate::delta_time;
	});

	batch_render_sprites((u8)last_atlas_used);

	_text_renderer.render_text();
	batch_render_sprites((u8)last_atlas_used);

	SDL_GL_SwapWindow(gamestate::window->get_sdl_window());
	
	if(input::get_key_pressed((u8)SDLK_RIGHT))
		camera->position.x += 32 * gamestate::delta_time;
	if(input::get_key_pressed((u8)SDLK_LEFT))
		camera->position.x -= 32 * gamestate::delta_time;
	if(input::get_key_pressed((u8)SDLK_UP))
		camera->position.y -= 32 * gamestate::delta_time;
	if(input::get_key_pressed((u8)SDLK_DOWN))
		camera->position.y += 32 * gamestate::delta_time;

	camera->update();
}

Renderer::~Renderer() {
	free(sprite_batcher.vbo_data);
	delete camera;
	delete atlasses.tileset;
}

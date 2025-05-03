#include "renderer.h"
#include "../core/state.h"
#include "camera.h"
#include "gfx.h"
#include "shader.h"
#include "triangle.h"
#include "vao.h"
#include "vbo.h"
#include <stdio.h>

#include "../../include/stb/stb_image.h"

OrthoCamera *camera;
Shader shader;
Shader spr_shader;
Triangle *triangle;

Shader batch_shader;
VAO b_vao;
VBO b_vbo;

static GLuint _load_texture(const char *path) {
	int w, h, ch;
	GLuint id;

	stbi_set_flip_vertically_on_load(true);
	u8 *raw_bytes = stbi_load("./res/img.png", &w, &h, &ch, STBI_rgb_alpha);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(raw_bytes);

	return id;
}

Renderer create_renderer(SDL_Window *window) {
	
	const bool debug_gl_specs = true;

	Renderer self = (Renderer) {
		.vsync_state = 0,
		.far_plane = 10.f,
		.near_plane = 0.1f
	};

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
	
	if(debug_gl_specs) {
		printf(
			"vendor:\t\t%s\nrenderer:\t%s\nversion:\t%s\nlang_version:\t%s\n",
			glGetString(GL_VENDOR),
			glGetString(GL_RENDERER),
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION)
		);
	}

	GLfloat tri_vec_pos[9] = {
		0.0, 0.0, 0.0,
		0.5, 0.0, 0.0,
		0.0, 0.5, 0.0
	};

	GLfloat tri_vec_color[9] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
	
	triangle = create_triangle(tri_vec_pos, tri_vec_color);

	shader = create_shader("./res/shaders/raw.vert", "./res/shaders/raw.frag");
	spr_shader = create_shader(
		"./res/shaders/texture.vert", 
		"./res/shaders/texture.frag"
	);

	batch_shader = create_shader(
		"./res/shaders/b.vert", 
		"./res/shaders/b.frag"
	);
	camera = create_ortho_camera((vec2s){{-1.0f, -1.0f}}, (vec2s){{1.0f, 1.0f}});

	b_vao = create_vao();
	b_vbo = create_vbo(GL_ARRAY_BUFFER);
	
	/* position | uv */
	f32 pos[40] = {
		-0.75f, -0.25f, 0.f, 	0, 0,
		-0.25f, -0.25f, 0.f, 	0.5f, 0,
		-0.25f, 0.25f, 0.f,	0.5, 1,
		-0.75f, 0.25f, 0.f,	0, 1,

		0.25f, -0.25f, 0.f, 	0.5f, 0,
		0.75f, -0.25f, 0.f, 	1, 0,
		0.75f, 0.25f, 0.f,     	1, 1,
		0.25f, 0.25f, 0.f,      0.5f, 1,

	};

	vbo_buffer(&b_vbo, pos, 0, sizeof(f32) * 40);
	vao_attr(&b_vao, &b_vbo, 0, 3, GL_FLOAT, 5 * sizeof(f32), 0);
	vao_attr(&b_vao, &b_vbo, 1, 2, GL_FLOAT, 5 * sizeof(f32), sizeof(f32) * 3);

	u32 img = _load_texture("./res/img.png");
	
	return self;
}


void renderer_prepare() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0,0, game_state.window.width, game_state.window.height);
	glClearColor(0.1f, 0.1f, 0.5f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void render(Renderer *self, SDL_Window *window) {
	renderer_prepare();
	
	mat4 model = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	vec3 scale = {1.5, 1.5, 1.5};
	
	mat4 rotation_matrix;
	glm_rotate_y(model, 0.0f, rotation_matrix); // should use glm_rotate_at();
	
	mat4 translation_matrix;
	glm_translate_to(model, (vec3){0.0f, 0.0f, 0.0f}, translation_matrix);
	
	mat4 scale_matrix;
	glm_scale_to(model, scale, scale_matrix);
	
	// translate -> rotate -> scale
	glm_mat4_mulN((mat4 *[]){&model, &translation_matrix, &rotation_matrix, &scale_matrix}, 4, model);

	/* shader_bind(&shader); */
	/* shader_uniform_mat4(&shader, "u_model", model); */
	/* shader_uniform_mat4s(&shader, "u_perspective", camera->view_proj.projection); */
	/* shader_uniform_mat4s(&shader, "u_view", camera->view_proj.view); */
	/*  */
	/* triangle_render(triangle); */

	shader_bind(&spr_shader);
	shader_uniform_mat4(&spr_shader, "u_model", model);
	shader_uniform_mat4s(&spr_shader, "u_perspective", camera->view_proj.projection);
	shader_uniform_mat4s(&spr_shader, "u_view", camera->view_proj.view);
	shader_uniform_int(&spr_shader, "tex", 0);

	/* ecs_event(&game_state.ecs, ECS_RENDER); */

	// test sprite batching
	u32 atlas_handle = _load_texture("./res/img.png");
	glBindTexture(0, atlas_handle);

	shader_bind(&batch_shader);
	shader_uniform_int(&batch_shader, "u_sprite_atlas", 0);
	vao_bind(&b_vao);

	u32 ids[12] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, ids);
	
	SDL_GL_SwapWindow(window);

	ortho_camera_update(camera);
}


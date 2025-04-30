#include "renderer.h"
#include "../core/state.h"
#include "camera.h"
#include "shader.h"
#include "triangle.h"
#include <stdio.h>

OrthoCamera *camera;
Shader shader;
Triangle *triangle;

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
	camera = create_ortho_camera((vec2s){{-1.0f, -1.0f}}, (vec2s){{1.0f, 1.0f}});

	return self;
}

void renderer_prepare() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0,0, game_state.window.width, game_state.window.height);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

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

	shader_uniform_mat4s(&shader, "u_perspective", camera->view_proj.projection);
	shader_uniform_mat4s(&shader, "u_view", camera->view_proj.view);
	
	shader_bind(&shader);
	triangle_render(triangle);

	SDL_GL_SwapWindow(window);

	ortho_camera_update(camera);
}


#include "renderer.h"
#include "window.h"

Renderer renderer;

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

	return self;
}

void renderer_prepare() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0,0, window.width, window.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void render(Renderer *self, SDL_Window *window) {

}


#include "window.h"

Window window;

Window create_window(const char *title, i32 w, i32 h) {

	Window self;

	self.width = w;
	self.height = h;
	self.aspect = (f32)(w) / (f32)(h);

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		THROW_ERR("failed to init SDL\n");
	}
	self.sdl_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		SDL_WINDOW_OPENGL
	);

	if(!self.sdl_window) {
		THROW_ERR("failed to init SDL Window\n");
	}

	return self;
}

void destroy_window(Window *self) {
	SDL_DestroyWindow(self->sdl_window);
	SDL_Quit();
}

#include "window.hpp"
#include <SDL2/SDL_video.h>

Window::Window(const char *title, i32 width, i32 height) {
	this->height = height;
	this->width = width;

	this->aspect = (f32) width / (f32) height;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		THROW_ERR("failed to init SDL!");	
	}

	this->sdl_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if(!this->sdl_window) {
		THROW_ERR("failed to create window");
	}
}

Window::~Window() {
	SDL_DestroyWindow(this->sdl_window);
	SDL_Quit();
}

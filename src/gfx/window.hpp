#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.hpp"

class Window {
private: 
	SDL_Window *sdl_window;
public:
	i32 width, height;
	f32 aspect;

	Window(const char *title, i32 width, i32 height);
	~Window();

	SDL_Window *get_sdl_window() {
		return this->sdl_window;
	};
};

#endif // WINDOW_H

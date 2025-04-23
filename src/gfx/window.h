#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "renderer.h"

typedef struct Window {
	SDL_Window *sdl_window;
	i32 width, height;
	f32 aspect;
} Window;

Window create_window(const char *title, i32 w, i32 h);
void destroy_window(Window *self);

extern Window window;

#endif // WINDOW_H

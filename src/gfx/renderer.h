#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"

typedef struct Renderer {
	SDL_GLContext context;
	bool vsync_state;
	f32 near_plane, far_plane;
} Renderer;

extern Renderer renderer;

Renderer create_renderer(SDL_Window *window);
void destroy_renderer(Renderer *self);
void render_prepare(void);
void render(Renderer *self, SDL_Window *window);

#endif // RENDERER_H

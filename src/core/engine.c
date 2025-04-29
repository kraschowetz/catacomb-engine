#include "engine.h"

#include "../util/util.h"
#include "time.h"
#include "input.h"
#include "../gfx/window.h"

#include "../ecs/ecs.h"

bool _running = false;

/*
 * this shit segfaults when calling SDL_QUIT
 * segfault happens @ ??(), prolly in ecs_destroy, or sum with stbi image impl
 * fuck...
 *
 * i fucking forgot to update ECS_COMP_LAST to C_SPRITE
 * thus ecs was calling NULL as C_Sprite subscribers
 *
 * i am a total moron
 */

void _poll_events(void) {
	SDL_Event e;
	SDL_PollEvent(&e);

	switch(e.type) {
		case SDL_QUIT:
			_running = false;
			break;
	}

	update_input_state(e);
}

void run_engine(void) {
	_running = true;
	
	window = create_window("catacomb engine", 800, 600);
	renderer = create_renderer(window.sdl_window);
	
	ECS ecs;
	ecs_init(&ecs);
	
	// just toying with ECS
	Entity player = ecs_new(&ecs);
	ecs_add(player, C_PRINTA, (C_PrintA){.my_string="sou um componente!"});
	ecs_add(player, C_TRANSFORM);
	ecs_add(player, C_SPRITE);

	while(_running) {
		_poll_events();

		// render window
		render(&renderer, window.sdl_window);
		/* ecs_event(&ecs, ECS_TICK); */

		update_global_time();
	}
}


void stop_engine(void) {

}

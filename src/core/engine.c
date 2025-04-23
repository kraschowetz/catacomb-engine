#include "engine.h"

#include "../util/util.h"
#include "time.h"
#include "input.h"
#include "../gfx/window.h"

bool _running = false;

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

	while(_running) {
		_poll_events();

		// render window
		render(&renderer, window.sdl_window);

		update_global_time();
	}
}


void stop_engine(void) {

}

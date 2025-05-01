#include "engine.h"

#include "../util/util.h"
#include "time.h"
#include "input.h"
#include "../gfx/window.h"

#include "state.h"
#include "../ecs/ecs.h"

GameState game_state;
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
	
	game_state.window = create_window("catacomb engine", 800, 600);
	game_state.renderer = create_renderer(game_state.window.sdl_window);
	
	ecs_init(&game_state.ecs);
	
	// just toying with ECS
	Entity player = ecs_new(&game_state.ecs);
	ecs_add(player, C_PRINTA, (C_PrintA){.my_string="sou um componente!"});
	ecs_add(
		player,
		C_TRANSFORM,
		((C_Transform) {
			.position = glms_vec2_zero(),
			.scale = glms_vec2_one(),
			.rotation = 0.f
		})
	);
	ecs_add(player, C_SPRITE);

	while(_running) {
		_poll_events();

		ecs_event(&game_state.ecs, ECS_TICK);
		render(&game_state.renderer, game_state.window.sdl_window);

		update_global_time();
	}
}


void stop_engine(void) {

}

#include "engine.h"

#include "../util/util.h"
#include "time.h"
#include "input.h"
#include "../gfx/window.h"

#include "state.h"
#include "../ecs/ecs.h"
#include "../ecs/c_sprite.h"

GameState game_state;
bool _running = false;

// creates ~16.5k sprites & transforms;
// @ my current specs it runs at ~15fps :(
// gotta find a way to optimize this
// goal is ~25fps on stress

/*
 * consider:
 * 	- sprite atlasses
 * 	- batching draw calls
 * 	- minimizing gl shader calls
 */

const bool stress_test = 0;

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
	ecs_add(player, C_SPRITE, ((C_Sprite){.z_index=-1}));

	Entity e = ecs_new(&game_state.ecs);
	ecs_add(
		e,
		C_TRANSFORM,
		((C_Transform) {
			.position = (vec2s){{-0.5f, 0.f}},
			.scale = (vec2s){{0.75f, 0.75f}},
			.rotation = 0.f
		})
	);
	ecs_add(e, C_SPRITE, ((C_Sprite){.z_index=0}));
	
	if(stress_test) {
		for(int i = 0; i < 0xFFFF; i++) {
			Entity _ = ecs_new(&game_state.ecs);
			ecs_add(
				_,
				C_TRANSFORM,
				((C_Transform) {
					.position = (vec2s){{-0.5f, 0.f}},
					.scale = (vec2s){{0.75f, 0.75f}},
					.rotation = 0.f
				})
			);
			ecs_add(_, C_SPRITE, ((C_Sprite){.z_index=rand() % 10}));
		}
	}

	/*
	* the engine need to order the sprites from less_z to great_z
	* for z_indices to work
	*/

	while(_running) {
		_poll_events();

		ecs_event(&game_state.ecs, ECS_TICK);
		render(&game_state.renderer, game_state.window.sdl_window);

		update_global_time();
	}
}


void stop_engine(void) {

}

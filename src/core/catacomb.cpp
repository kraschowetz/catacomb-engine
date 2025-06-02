#include "catacomb.hpp"

#include "gamestate.hpp"
#include "input.hpp"

#include "../ecs/components.hpp"
#include "../gfx/gfx.hpp"
#include <glm/geometric.hpp>

static bool _running;

static void _poll_events() {
	SDL_Event e;
	SDL_PollEvent(&e);

	if(e.type == SDL_QUIT) {
		_running = false;
	}

	input::update(e);
}

static void _update_gamestate_time() {
	using namespace gamestate;
	static f32 last_time = 0.f;
	
	frames++;
	delta_time = delta_timer.get_time();
	delta_timer.init();
	program_time = program_timer.get_time();
	
	if(program_time > last_time) {
		last_time += 1.f;
		std::cout << frames << "/s\n";
		frames = 0;
	}
}

void catacomb::init() {
	_running = true;
	gamestate::window = new Window(
		"catacomb engine",
		800, 600
	);
	gamestate::renderer = new Renderer();
	
	gamestate::program_timer.init();

	gamestate::ecs = new ECS();

	EntityID e1 = gamestate::ecs->create_entity();
	EntityID e2 = gamestate::ecs->create_entity();

	gamestate::ecs->add<Sprite>(e1, {
		.z_index = 0,
		.atlas_coords = {0, 0},
		.size = {16, 16},
		.atlas_index = 0
	});
	gamestate::ecs->add<Transform>(e1, {
		.position = {200, 200},
		.scale = {10, 10},
		.rotation = 0
	});

	gamestate::ecs->add<Sprite>(e2, {
		.z_index = 1,
		.atlas_coords = {0, 0},
		.size = {16, 16},
		.atlas_index = 1
	});
	gamestate::ecs->add<Transform>(e2, {
		.position = {300, 200},
		.scale = {10, 10},
		.rotation = 0
	});
	
	EntityID _text = gamestate::ecs->create_entity();
	gamestate::ecs->add<Text>(_text, {
		.text = "Gulliver",
		.size = 8,
		.bounds = {0, 0}
	});
	gamestate::ecs->add<Transform>(_text, {
		.position = {0, 48},
		.scale = {3, 3},
		.rotation = 0
	});

	EntityID player = gamestate::ecs->create_entity();
	gamestate::ecs->add<Transform>(player, {
		.position = {0, 0},
		.scale = {3, 3},
		.rotation = 0
	});
	gamestate::ecs->add<Sprite>(player, {
		.z_index = 1,
		.atlas_coords = {0, 0},
		.size = {16, 16},
		.atlas_index = 1
	});
	gamestate::ecs->add<Camera2D>(player,
		Camera2D(
		       {0, 0},
		       {gamestate::window->width, gamestate::window->height}
		)
	);

	while(_running) {
		_poll_events();

		{	// player.update();
			Transform& transform = gamestate::ecs->get<Transform>(player);
			
			glm::vec2 direction = glm::vec2(
				input::get_key_axis(
					(u8)SDLK_LEFT, (u8)SDLK_RIGHT
				),
				input::get_key_axis(
					(u8)SDLK_UP, (u8)SDLK_DOWN
				)
			);
			
			if(glm::length(direction) > 0) {
			transform.position += 
				glm::normalize(direction) * 256.f * gamestate::delta_time;
			}
		}

		gfx::render_cycle();

		_update_gamestate_time();
	}
}

void catacomb::stop() {
	delete gamestate::window;
	delete gamestate::renderer;
	delete gamestate::ecs;
}

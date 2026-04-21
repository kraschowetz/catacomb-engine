#include "cat/config.hpp"
#include "cat/core/input.hpp"
#include "cat/gfx/canvas.hpp"
#include "cat/gfx/sdl_canvas.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/util/logger.hpp"
#include "cat/ecs/ecs.hpp"
#include <cat/core/engine.hpp>
#include <cat/util/benchmark.hpp>
#include <iostream>

#include <unistd.h>

int main(int argc, char** argv)
{
	std::cout << "hello!\n";
	cat::CanvasInfo info
	{
		.title = "Sandbox",
		.width = 800,
		.height = 600,
		.version = {4, 6}
	};
	cat::SdlCanvas canvas {info};

	cat::engine::init_internals();
	
	while(!cat::input::has_queued_exit())
	{
		if(cat::input::is_key_just_released(cat::eKeyType::A))
		{
			LOG_TEXT("A was just released!\n");
		}

		if(cat::input::is_key_just_pressed(cat::eKeyType::B))
		{
			LOG_TEXT("B was just pressed!\n");
		}

		if(cat::input::is_key_pressed(cat::eKeyType::C))
		{
			LOG_TEXT("C was pressed!\n");
		}

		cat::engine::update_internals();
	}

	for(int i = 0; i < 10; ++i)
	{
		CAT_BENCH_SCOPE("update loop", bench_marker);
		sleep(1);
	}

	// pretty basic ECS use
	cat::ECS ecs;

	ecs.register_component_index<int>();
	cat::EntityID a = ecs.create_entity();
	ecs.add_component<int>(a, 12);

	auto view = ecs.view<int>();

	view.foreach([](int i){LOG_TEXTF("%d\n", i);});
	ecs.remove_component<int>(a);
	ecs.delete_entity(a);

	cat::engine::destroy_internals();

	CAT_BENCH_DISPLAY(false);

	return 0;
}

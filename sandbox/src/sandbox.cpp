#include "cat/config.hpp"
#include "cat/core/input.hpp"
#include "cat/gfx/canvas.hpp"
#include "cat/gfx/sdl_canvas.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/util/logger.hpp"
#include "cat/ecs/ecs.hpp"
#include <iostream>

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

	cat::Input::init();
	
	while(!cat::Input::has_queued_exit())
	{
		if(cat::Input::is_key_just_released(cat::eKeyType::A))
		{
			LOG_TEXT("A was just released!\n");
		}

		if(cat::Input::is_key_just_pressed(cat::eKeyType::B))
		{
			LOG_TEXT("B was just pressed!\n");
		}

		if(cat::Input::is_key_pressed(cat::eKeyType::C))
		{
			LOG_TEXT("C was pressed!\n");
		}

		cat::Input::update();
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

	cat::Input::quit();

	return 0;
}

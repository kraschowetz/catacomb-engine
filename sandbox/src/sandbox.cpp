#include "cat/core/input.hpp"
#include "cat/gfx/canvas.hpp"
#include "cat/gfx/sdl_canvas.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/util/logger.hpp"
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

	cat::Input::quit();

	return 0;
}

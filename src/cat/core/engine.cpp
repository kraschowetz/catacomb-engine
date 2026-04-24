#include <cat/core/engine.hpp>

#include <cat/core/input_manager.hpp>
#include <cat/util/chrono.hpp>

#include <cat/util/logger.hpp>

void cat::engine::init_internals()
{
	chrono::init();
	input::init();
}

void cat::engine::update_internals()
{
	input::update();
	chrono::update();
}

void cat::engine::destroy_internals()
{
	input::quit();
}

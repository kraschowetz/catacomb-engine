#pragma once

//! @file chrono.hpp implements a wrapper struct to time-based operations

#include <chrono>	// IWYU pragma: export
#include <cat/config.hpp>

using seconds_t = f32;
using millis_t = u64;

namespace cat
{

//! @brief chrono is designed to be used as a namespace
//! 	for that reason, chrono is a struct where all of its
//!	members are static
struct chrono
{
public:
	//! @brief get number of seconds passed since the start of the engine
	static seconds_t current_seconds();

	//! @brief get number of milliseconds passed since the start of the engine
	static millis_t current_millis();

	//! @brief get the current delta_time in seconds
	static seconds_t get_delta();

private:
	//! @brief updates the chrono state, called internaly via Engine::update_internals()
	static void update();

	//! @brief starts the chrono state, called internaly via Engine::init_internals()
	static void init();

private:
	static millis_t s_last_millis;
	static millis_t s_epoch;
	static seconds_t s_delta;

	friend struct engine;
};

}

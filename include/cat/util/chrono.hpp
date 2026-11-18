#pragma once

//! @file chrono.hpp implements a wrapper struct to time-based operations

#include <chrono>	// IWYU pragma: export
#include <cat/config.hpp>

using seconds_t = f32;
using millis_t = u64;

namespace cat
{

struct Chrono
{
public:
    Chrono();

	// brief get number of seconds passed since the construction of Chrono
	seconds_t current_seconds() const;

	// brief get number of milliseconds passed since the construction of Chrono
	millis_t current_millis() const;

	// brief get the current delta_time in seconds
	seconds_t get_delta() const;

    // returns the ammount of frames rendered -- or calls to update() -- in the previous second
    u16 get_fps() const;

	// brief updates the chrono state, called internaly via Engine::update_internals()
	void update();

private:
	millis_t m_last_millis;
	millis_t m_epoch;
	seconds_t m_delta;
    millis_t m_last_second;// store when the last fps flush call hapened 
    u16 m_current_frames;   // store how many calls `update()` happened in the current second
    u16 m_last_frames;      // store how many calls `update()` happened in the last second
};
}

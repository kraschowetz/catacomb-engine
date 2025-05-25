#include "timer.hpp"

#include <SDL2/SDL_timer.h>

void Timer::init() {
	this->start_ticks = SDL_GetTicks();
}

u32 Timer::get_ticks() {
	return SDL_GetTicks() - this->start_ticks;
}

f32 Timer::get_time() {
	return (f32)(SDL_GetTicks() - this->start_ticks) / 1000.f;
}

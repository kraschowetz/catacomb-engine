#include "timer.h"
#include <SDL2/SDL_timer.h>

f32 delta_time = 0.0f;

Timer timer_start() {
	Timer self;
	self.start_time = SDL_GetTicks();

	return self;
}

u32 timer_get_ticks(const Timer *self) {
	return SDL_GetTicks() - self->start_time;
}

f32 timer_get_time(const Timer *self) {
	return (f32)(SDL_GetTicks() - self->start_time) / 1000.f;
}

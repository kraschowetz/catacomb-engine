#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../gfx/window.hpp"
#include "../gfx/renderer.hpp"
#include "../ecs/ecs.hpp"
#include "timer.hpp"

namespace gamestate {
	inline Window *window = nullptr;
	inline Renderer *renderer = nullptr;
	inline ECS *ecs = nullptr;
	inline f32 program_time, delta_time;
	inline Timer program_timer, delta_timer;
	inline u16 frames;
}

#endif // GAMESTATE_H

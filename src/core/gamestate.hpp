#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../gfx/window.hpp"
#include "../gfx/renderer.hpp"
#include "../ecs/ecs.hpp"
#include "timer.hpp"

namespace gamestate {
	extern Window *window;
	extern Renderer *renderer;
	extern ECS *ecs;
	extern f32 program_time, delta_time;
	extern Timer program_timer, delta_timer;
	extern u16 frames;
}

#endif // GAMESTATE_H

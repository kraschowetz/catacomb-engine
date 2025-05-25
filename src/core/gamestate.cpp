#include "gamestate.hpp"
#include <cmath>

namespace gamestate {
	Window *window(nullptr);
	Renderer *renderer(nullptr);
	ECS *ecs(nullptr);
	f32 program_time(0.f);
	f32 delta_time(0.f);
	Timer program_timer, delta_timer;
	u16 frames(0);
}

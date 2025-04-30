#ifndef STATE_H
#define STATE_H

#include "../ecs/ecs.h"
#include "../gfx/window.h"
#include "../gfx/renderer.h"

typedef struct GameState {
	ECS ecs;
	Window window;
	Renderer renderer;
} GameState;

/* 
 * global-ish game state
 * initialized @ engine.c
 */

extern GameState game_state;

#endif // STATE_H

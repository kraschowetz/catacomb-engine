#include "c_printa.h"
#include "ecs.h"
#include "../core/input.h"
#include <SDL2/SDL_keycode.h>

static void tick(C_PrintA *self, Entity entity) {
	if(get_key_just_pressed(SDLK_SPACE))
		printf("%s\n", self->my_string);
}

void c_printa_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_PRINTA,
		C_PrintA,
		ecs,
		((ECS_System) {
			.init = NULL,
			.destroy = NULL,
			.render = NULL,
			.update = NULL,
			.tick = (ECS_Subscriber) tick
		})
	);
}

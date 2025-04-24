#include "c_printa.h"
#include "ecs.h"

static void tick(C_PrintA *self, Entity entity) {
	printf("%c\n", self->my_character);
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

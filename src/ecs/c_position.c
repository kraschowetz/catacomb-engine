#include "c_position.h"
#include "ecs.h"

void c_position_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_POSITION,
		C_Position,
		ecs,
		((ECS_System) {
			.subscribers = {
				NULL, NULL, NULL, NULL
			}
		})
	);
}

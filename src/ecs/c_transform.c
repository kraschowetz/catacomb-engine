#include "c_transform.h"
#include "ecs.h"

void c_transform_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_TRANSFORM,
		C_Transform,
		ecs,
		((ECS_System) {
			.subscribers = {
				NULL, NULL, NULL, NULL
			}
		})
	);
}

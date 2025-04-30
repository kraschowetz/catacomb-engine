#include "c_sprite.h"
#include "ecs.h"

/* #define STB_IMAGE_IMPLEMENTATION */
/* #include "../../include/stb/stb_image.h" */

static void _render(C_Sprite *self, Entity entity) {
	;;
}

static void _init(C_Sprite *self, Entity entity) {
	;;
}

static void _destroy(C_Sprite *self, Entity entity) {
	;;
}

void c_sprite_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_SPRITE,
		C_Sprite,
		ecs,
		((ECS_System) {
			.init = (ECS_Subscriber) _init,
			.destroy = (ECS_Subscriber) _destroy,	
			.render = (ECS_Subscriber) _render,
			.tick = NULL
		})
	);
}

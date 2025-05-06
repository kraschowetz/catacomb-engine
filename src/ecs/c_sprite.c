#include "c_sprite.h"
#include "c_transform.h"
#include "ecs.h"
#include "ecs_components.h"
#include "../core/time.h"
#include "../core/state.h"
#include "../gfx/renderer.h"
#include <cglm/struct/vec2.h>

static void _render(C_Sprite *self, Entity entity) {
	C_Transform *transform = ecs_get(entity, C_TRANSFORM);
	if(!transform) {
		C_Transform t = (C_Transform) {
			.position = ((vec2s) {{
				(f32) game_state.window.width / 2,
				(f32) game_state.window.height / 2
			}}),
			.scale = GLMS_VEC2_ONE
		};
		renderer_add_sprite_to_batch(	
			&game_state.renderer,
			self,
			&t
		);
		return;
	}

	renderer_add_sprite_to_batch(	
		&game_state.renderer,
		self,
		transform
	);
}

static void _init(C_Sprite *self, Entity entity) {
	if(self->atlas_coords.x < 0) {
		self->atlas_coords.x = 0;
	}
	if(self->atlas_coords.y < 0) {
		self->atlas_coords.y = 0;
	}

	if(self->size.x <= 0) {
		self->size.x = 1;
	}
	if(self->size.y <= 0) {
		self->size.y = 0;
	}
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

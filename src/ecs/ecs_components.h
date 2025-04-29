#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "c_transform.h" // IWYU pragma: export
#include "c_printa.h"	// IWYU pragma: export

struct ECS;

#define _ECS_DECLARE_COMP(_name) \
	extern void _name##_init(struct ECS *ecs); \
	_name##_init(ecs);

static inline void _ecs_init_internals(struct ECS *ecs) {
	_ECS_DECLARE_COMP(c_transform);
	_ECS_DECLARE_COMP(c_printa);
	_ECS_DECLARE_COMP(c_sprite);
}

#define ECS_COMP_LAST C_SPRITE
typedef enum ECS_Component {
	C_TRANSFORM = 0,	// position, scale & rotation
	C_PRINTA,		// remove this
	C_SPRITE,		// 2D sprite
	C_CAMERA,		// orthogonal 2D camera
	C_MOVEMENT,		// basic tile based movement

} ECS_Component;

#endif // ECS_COMPONENTS_H

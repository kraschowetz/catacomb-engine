#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "c_position.h" // IWYU pragma: export
#include "c_printa.h"	// IWYU pragma: export

struct ECS;

#define _ECS_DECLARE_COMP(_name) \
	extern void _name##_init(struct ECS *ecs); \
	_name##_init(ecs);

static inline void _ecs_init_internals(struct ECS *ecs) {
	_ECS_DECLARE_COMP(c_position);
	_ECS_DECLARE_COMP(c_printa);
}

#define ECS_COMP_LAST C_PRINTA
typedef enum ECS_Component {
	C_POSITION = 0,	// convert to C_TRANSFORM (position & scale)
	C_PRINTA,	// remove this
	C_CAMERA,	// orthogonal 2D camera
	C_SPRITE,	// 2D sprite
	C_MOVEMENT,	// basic tile based movement

} ECS_Component;

#endif // ECS_COMPONENTS_H

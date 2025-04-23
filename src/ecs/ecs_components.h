#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

/* #include "c_position.h" */

struct ECS;

#define _ECS_DECLARE_COMP(_name) \
	extern void _name##_init(struct ECS *ecs); \
	_name##_init(ecs);

static inline void _ecs_init_internals(struct ECS *ecs) {
	_ECS_DECLARE_COMP(c_position);
}

#define ECS_COMP_LAST C_PRINTA
typedef enum ECS_Component {
	C_POSITION = 0,
	C_PRINTA
} ECS_Component;

#endif // ECS_COMPONENTS_H

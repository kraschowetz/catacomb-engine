#ifndef ECS_H
#define ECS_H

#include "../util/util.h"
#include "ecs_components.h"
#include "../util/bitmap.h"

#define ECS_REGISTER(_id, _comp, _ecs, _system) \
	_ecs_register_internal(_id, sizeof(_comp), _ecs, _system)

typedef u64 ECS_Tag;

#define ECS_EVENT_LAST ECS_TICK
typedef enum ECS_Event {
	ECS_INIT = 0,
	ECS_DESTROY,
	ECS_RENDER,
	ECS_UPDATE,
	ECS_TICK
} ECS_Event;

typedef void (*ECS_Subscriber)(void*, Entity);

typedef union ECS_System {
	struct {
		ECS_Subscriber init, destroy, render, update, tick;
	};
	ECS_Subscriber subscribers[ECS_EVENT_LAST + 1];
} ECS_System;

typedef struct ECS_ComponentList {
	ECS_System system;
	void *components;
	size_t component_size;
} ECS_ComponentList;

typedef enum ECS_TagValue {
	ECS_TAG_USED = 1 << 0
} ECS_TagValue;

typedef struct ECS {
	ECS_ComponentList lists[ECS_COMP_LAST + 1];
	EntityID *ids;
	Bitmap used;
	size_t capacity;
	EntityID next_entity_id;
} ECS;

void _ecs_register_internal(
	ECS_Component id, size_t component_size,
	ECS *ecs, ECS_System system
);

void _ecs_add_internal(Entity entity, ECS_Component component_id, void *val);

// wtf...
#define _ecs_add3(e, c, v) ({ __typeof__(v) _v = (v); _ecs_add_internal((e), (c), &_v); })
#define _ecs_add2(e, c) _ecs_add_internal((e), (c), NULL)

#define _ecs_add_overload(_1,_2,_3,NAME,...) NAME
#define ecs_add(...) _ecs_add_overload(__VA_ARGS__, _ecs_add3, _ecs_add2)(__VA_ARGS__)

void ecs_event(ECS *self, ECS_Event event);
Entity ecs_new(ECS *self);
void ecs_delete(ECS *self, Entity entity);
void ecs_remove(Entity entity, ECS_Component component);
bool ecs_has(Entity entity, ECS_Component component);
void *ecs_get(Entity entity, ECS_Component component);
void ecs_init(ECS *self);
void ecs_destroy(ECS *self);

#endif // ECS_H

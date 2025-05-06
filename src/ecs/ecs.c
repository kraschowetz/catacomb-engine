// discretização
#include "ecs.h"
#include "ecs_components.h"
#include <cglm/common.h>
#include <stdlib.h>
#include <string.h>

#define ECS_TAG_SIZE 16
#define ECS_TAG(_p) *((ECS_Tag*) (((u8*) (_p)) - ECS_TAG_SIZE))
#define ECS_PTAG(_p) ((ECS_Tag*) (((u8*) (_p)) - ECS_TAG_SIZE))
#define ECSCL_ELEMENT_SIZE(_plist) ((_plist)->component_size + ECS_TAG_SIZE)
#define ECSCL_GET(_plist, _i) ({\
        ECS_ComponentList *_pl = (_plist);\
        (u8*)(((_pl)->components)) + ((_i) * ECSCL_ELEMENT_SIZE(_pl)) + ECS_TAG_SIZE;\
    })

void _ecs_register_internal(
	ECS_Component id, size_t component_size,
	ECS *ecs, ECS_System system
) {
	ECS_ComponentList list = {
		.system = system,
		.component_size = component_size
	};

	list.components = calloc(ecs->capacity, ECSCL_ELEMENT_SIZE(&list));

	ecs->lists[id] = list;
}

void ecs_event(ECS *self, ECS_Event event) {
	for(size_t i = 0; i <= ECS_COMP_LAST; i++) {
		ECS_ComponentList *list = &self->lists[i];

		ECS_Subscriber subs = list->system.subscribers[event];
		if(!subs) {
			continue;
		}

		for(size_t j = 0; j < self->capacity; j++) {
			void *comp = ECSCL_GET(list, j);
			if(ECS_TAG(comp) & ECS_TAG_USED) {
				subs(
					comp,
					(Entity){
						.id = self->ids[i],
						.index = j,
						.ecs = self
					}
				);
			}
		}
	}
}

Entity ecs_new(ECS *self) {
	size_t i = 0;
	for(; i < self->capacity; i += 64) {
		if(((u64*) self->used)[i/64] != 0xFFFFFFFFFFFFFFFF) {
			break;
		}
	}

	if(i == self->capacity) {
		size_t old_cap = self->capacity;
		self->capacity *= 2;

		// realoc bitmap & clear new alloc
		self->used = bitmap_realloc(self->used, self->capacity);
		memset(
			(u8*) self->used + BITMAP_SIZE_TO_BYTES(old_cap),
			0,
			BITMAP_SIZE_TO_BYTES(self->capacity)-BITMAP_SIZE_TO_BYTES(old_cap)
		);

		//realoc idmap & clear new alloc
		self->ids = realloc(self->ids, self->capacity * sizeof(u64));
		memset(
			(u8*) self->ids + old_cap,
			0,
			(self->capacity - old_cap) * sizeof(u64)
		);

		for(size_t j = 0; j <= ECS_COMP_LAST; j++) {
			ECS_ComponentList *list = &self->lists[j];
			list->components = realloc(
				list->components,
				self->capacity * ECSCL_ELEMENT_SIZE(list)
			);

			memset(
				(u8*)list->components + (old_cap * ECSCL_ELEMENT_SIZE(list)),
				0,
				(self->capacity - old_cap) * ECSCL_ELEMENT_SIZE(list)
			);
		}
	}
	else {
		for(; i < self->capacity; i++) {
			if (!bitmap_get(self->used, i)) {
                		break;
        		}
		}
	}

	bitmap_set(self->used, i);

	EntityID id = self->next_entity_id++;
	self->ids[i] = id;

	return (Entity) {
		.id = id,
		.index = i,
		.ecs = self
	};
}

void ecs_delete(ECS *self, Entity entity) {
	assert(bitmap_get(self->used, entity.index));

	for(size_t i = 0; i < ECS_COMP_LAST; i++) {
		ECS_ComponentList *list = &self->lists[i];
		ECS_Subscriber destroy = list->system.destroy;

		void *component = ECSCL_GET(list, entity.index);
		*ECS_PTAG(component) &= (ECS_Tag) ~ECS_TAG_USED;

		if (destroy != NULL) {
        		destroy(component, entity);
        	}
	}

	bitmap_clear(self->used, entity.index);

	self->ids[entity.index] = ENTITY_NONE;
}

void _ecs_add_internal(Entity entity, ECS_Component component_id, void *val) {
	ECS_ComponentList *list = &entity.ecs->lists[component_id];
	ECS_Subscriber init = list->system.init;
	void *component = ECSCL_GET(list, entity.index);
	
	assert(!(ECS_TAG(component) & ECS_TAG_USED));
	*ECS_PTAG(component) |= ECS_TAG_USED;

	if (val != NULL) {
        	memcpy(component, val, list->component_size);
	}

	if (init != NULL) {
        	init(component, entity);
	}
}

void ecs_remove(Entity entity, ECS_Component component_id) {
	ECS_ComponentList *list = &entity.ecs->lists[component_id];
	ECS_Subscriber destroy = list->system.destroy;

	void *component = ECSCL_GET(list, entity.index);

	assert(ECS_TAG(component) & ECS_TAG_USED);
	*ECS_PTAG(component) &= (ECS_Tag) ~ECS_TAG_USED;

	if(destroy) {
		destroy(component, entity);
	}
}

bool ecs_has(Entity entity, ECS_Component component) {
	return ECS_TAG(ECSCL_GET(&entity.ecs->lists[component], entity.index)) & ECS_TAG_USED;
}

void *ecs_get(Entity entity, ECS_Component component) {
	assert(ecs_has(entity, component));
	return ECSCL_GET(&entity.ecs->lists[component], entity.index);
}

void ecs_init(ECS *self) {
	self->capacity = 64;
	self->ids = calloc(self->capacity, sizeof(EntityID));
	self->used = bitmap_calloc(self->capacity);
	self->next_entity_id = 1;

	_ecs_init_internals(self);
}

void ecs_destroy(struct ECS *self) {
	free(self->used);
	free(self->ids);

	for (size_t i = 0; i <= ECS_COMP_LAST; i++) {
		free(self->lists[i].components);
	}
}

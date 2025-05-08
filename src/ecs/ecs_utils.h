#ifndef ECS_UTILS_H
#define ECS_UTILS_H

#include "../util/util.h"

#define ECS_ARRAYLIST_INITIAL_CAP 8

typedef struct {
	void *raw;
	u64 length, comp_size;
	u64 capacity;
} ECS_ArrayList;

ECS_ArrayList create_ecs_arraylist(size_t comp_size);
void destroy_ecs_arraylist(ECS_ArrayList *self);

// adds val to list via memcpy;
void ecs_arraylist_add(ECS_ArrayList *self, void *val);
void ecs_arraylist_remove(ECS_ArrayList *self, u64 id);
void *ecs_arraylist_get(ECS_ArrayList *self, u64 id);
void ecs_arraylist_set(ECS_ArrayList *self, u64 id, void *val);

typedef struct { 
	ECS_ArrayList sparse;
	ECS_ArrayList dense;
	u64 size;
}
ECS_SparseSet;

#endif // ECS_UTILS_H

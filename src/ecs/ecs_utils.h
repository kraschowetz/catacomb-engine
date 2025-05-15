#ifndef ECS_UTILS_H
#define ECS_UTILS_H

#include "../util/arraylist.h"
#include "ecs_components.h"
#include "limits.h"

#define ECS_SPARSESET_PAGE_SIZE 3

typedef struct { 
	ArrayList *sparse;
	ArrayList *dense;
	u64 length;
}
ECS_SparseSet;

ECS_SparseSet create_ecs_sparseset(size_t comp_size);
void destroy_ecs_sparseset(ECS_SparseSet *self);
void *ecs_sparseset_get(ECS_SparseSet *self, Entity entity);
void ecs_sparseset_add(ECS_SparseSet *self, Entity entity, void *comp);
void ecs_sparseset_remove(ECS_SparseSet *self, Entity entity);

#endif // ECS_UTILS_H

#include "ecs_utils.h"
#include <stdlib.h>

ECS_ArrayList create_ecs_arraylist(size_t comp_size) {
	ECS_ArrayList self = {0};
	
	self.raw = calloc(
		ECS_ARRAYLIST_INITIAL_CAP,
		comp_size
	);
	
	self.comp_size = comp_size;
	self.length = 0;
	self.capacity = ECS_ARRAYLIST_INITIAL_CAP;

	return self;
}

static void _expand_ecs_arraylist(ECS_ArrayList *self) {
	void *new_data = calloc(
		self->capacity * 2,
		self->comp_size
	);

	memcpy(
		new_data,
		self->raw,
		self->capacity * self->comp_size
	);

	self->capacity *= 2;
	
	free(self->raw);
	self->raw = new_data;
}

void destroy_ecs_arraylist(ECS_ArrayList *self) {
	free(self->raw);
	self->raw = NULL;
}

void ecs_arraylist_add(ECS_ArrayList *self, void *val) {
	if(self->length == self->capacity) {
		_expand_ecs_arraylist(self);
	}
	
	void *memcpy_dest = (u8*)self->raw + (self->comp_size * self->length);
	memcpy(
		memcpy_dest,
		val,
		self->comp_size
	);

	self->length++;
}

void ecs_arraylist_remove(ECS_ArrayList *self, u64 id) {
	if(id >= self->length) {
		return;
	}

	void *dest = (u8*)self->raw + (self->comp_size * id);
	memset(
		dest,
		0,
		self->comp_size
	);

}

void *ecs_arraylist_get(ECS_ArrayList *self, u64 id) {
	if(id >= self->length) {
		return NULL;
	}

	return (void*)((u8*)self->raw + self->comp_size * id);
}

void ecs_arraylist_set(ECS_ArrayList *self, u64 id, void *val) {
	if(id >= self->length) {
		return;
	}

	void *memset_dest = (u8*)self->raw + self->comp_size * id;
	memset(memset_dest, 0, self->comp_size);
}

ECS_SparseSet create_ecs_sparseset(size_t comp_size) {
	ECS_SparseSet self = {0};
	
	self.dense = create_ecs_arraylist(comp_size);
	self.sparse = create_ecs_arraylist(sizeof(ECS_ArrayList*));
	self.length = 0;

	return self;
}

void ecs_sparseset_add(ECS_SparseSet *self, Entity entity, void *comp) {
	/* self->sparse; */
}

void destroy_ecs_sparseset(ECS_SparseSet *self) {
	destroy_ecs_arraylist(&self->sparse);
	destroy_ecs_arraylist(&self->dense);
}

void *ecs_sparseset_get(ECS_SparseSet *self, Entity entity) {
	if(entity.id > self->sparse.length) {
		return NULL;
	}

	return NULL;
}

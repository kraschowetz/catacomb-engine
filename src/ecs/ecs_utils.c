#include "ecs_utils.h"
#include "../util/staticlist.h"
#include "ecs_components.h"
#include <stdlib.h>

ECS_SparseSet create_ecs_sparseset(size_t comp_size) {
	ECS_SparseSet self;
	
	self.length = 0;
	self.dense = create_arraylist(comp_size);
	self.sparse = create_arraylist(sizeof(StaticList*));

	return self;
}

void destroy_ecs_sparseset(ECS_SparseSet *self) {
	/* need to free each StaticList* inside sparse */
	free(self->sparse);
	free(self->dense);
	self->sparse = NULL;
	self->dense = NULL;
}

void ecs_sparseset_add(ECS_SparseSet *self, Entity entity, void *comp) {
	arraylist_add(self->dense, comp);
	void *comp_ptr = arraylist_get(self->dense, self->dense->length - 1);
	
	const u64 last_page_index = self->sparse->length / ECS_SPARSESET_PAGE_SIZE;
	if(entity.id >= last_page_index) {
		for(u64 i = self->sparse->length; i < last_page_index; i++) {
			// add blank page
			StaticList *page = create_staticlist(
				sizeof(void*),
				ECS_SPARSESET_PAGE_SIZE
			);
			for(u8 j = 0; j < ECS_SPARSESET_PAGE_SIZE; j++) {
				staticlist_add(page, NULL);
			}
			arraylist_add(self->sparse, page);
		}
	}

	staticlist_set(
		arraylist_get(self->sparse, entity.id / ECS_SPARSESET_PAGE_SIZE),
		entity.id % ECS_SPARSESET_PAGE_SIZE,
		&comp_ptr
	);
}

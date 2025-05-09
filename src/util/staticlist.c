#include "staticlist.h"
#include "util.h"

StaticList *create_staticlist(u64 type_size, u64 capacity) {
	StaticList *self = malloc(sizeof(StaticList));
	
	self->data = calloc(capacity, type_size);
	self->capacity = capacity;
	self->type_size = type_size;
	self->length = 0;

	return self;
}

void destroy_staticlist(StaticList *self) {
	free(self->data);
	free(self);
}

void staticlist_add(StaticList *self, void *value) {
	if(self->length == self->capacity) {
		THROW_ERR("array index out of bounds error!\n");
		return;
	}

	void *dest = (u8*)self->data + (self->length * self->type_size);

	memcpy(dest, value, self->type_size);
	self->length++;
}

void staticlist_set(StaticList *self, u64 index, void *value) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return;
	}

	void *dest = (u8*)self->data + (index * self->type_size);

	memcpy(dest, value, self->type_size);
}

void *staticlist_get(StaticList *self, u64 index) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return NULL;
	}

	return (u8*)self->data + (index * self->type_size);
}

void staticlist_remove(StaticList *self, u64 index) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return;
	}

	void* removed_val = (u8*)self->data + (index * self->type_size);
	
	if(index == self->length - 1) {
		memset(removed_val, 0, self->type_size);
	}
	else {
		void *pushed_val = (u8*)self->data + ((index + 1) * self->type_size);
		memmove(
			removed_val,
			pushed_val,
			(self->length - index - 1) * self->type_size
		);
	}

	self->length--;
}

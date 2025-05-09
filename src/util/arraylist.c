#include "arraylist.h"
#include <string.h>

ArrayList *create_arraylist(u64 type_size) {
	ArrayList *self = malloc(sizeof(ArrayList));
	
	self->data = calloc(ARRAYLIST_INITIAL_CAP, type_size);

	self->type_size = type_size;
	self->length = 0;
	self->capacity = ARRAYLIST_INITIAL_CAP;

	return self;
}

void destroy_arraylist(ArrayList *self) {
	free(self->data);
	free(self);
}

static void _expand(ArrayList *self) {
	void *new_data = calloc(self->capacity * 2, self->type_size);
	memcpy(new_data, self->data, self->type_size * self->capacity);

	free(self->data);
	self->data = new_data;
	self->capacity *= 2;
}

static void _shrink(ArrayList *self) {
	void *new_data = calloc(self->capacity / 2, self->type_size);
	memcpy(new_data, self->data, self->type_size * (self->capacity / 2));

	free(self->data);
	self->data = new_data;
	self->capacity /= 2;
}

void arraylist_add(ArrayList *self, void *restrict value) {
	if(self->length == self->capacity) {
		_expand(self);
	}

	void *dest = (u8*)self->data + (self->type_size * self->length);
	memcpy(dest, value, self->type_size);
	
	self->length++;
}

void arraylist_remove(ArrayList *self, u64 index) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return;
	}

	void *removed_val = (u8*)self->data + (index * self->type_size);
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
	if(self->length <= self->capacity / 4 && self->capacity >= ARRAYLIST_INITIAL_CAP * 2) {
		_shrink(self);
	}
}

void arraylist_set(ArrayList *self, u64 index, void *value) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return;
	}

	void *dest = (u8*)self->data + (self->type_size * index);
	memcpy(dest, value, self->type_size);
}

void *arraylist_get(ArrayList *self, u64 index) {
	if(index >= self->length) {
		THROW_ERR("array index out of bounds error!\n");
		return NULL;
	}

	return (u8*)self->data + (self->type_size * index);
	
}

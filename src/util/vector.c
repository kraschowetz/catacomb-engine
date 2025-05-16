#include "vector.h"

#define INITIAL_VECTOR_CAP 8

static void *_expand(void *data, u64 size, u64 type) {
	void *new_data = calloc(size * 2, type);
	assert(new_data);
	memcpy(new_data, data, size * type);
	free(data);
	return new_data ;
}

static void *_shrink(void *data, u64 size, u64 type) {
	void *new_data = calloc(size / 2, type);
	memcpy(new_data, data, type * (size / 2));
	free(data);
	return new_data;
}

#define IMPL_VECTOR(type, vector)				\
vector *create_##vector(void) {					\
	vector *self = malloc(sizeof(vector));			\
	assert(self);						\
	self->data = calloc(INITIAL_VECTOR_CAP, sizeof(type));	\
	self->length = 0;					\
	self->capacity = INITIAL_VECTOR_CAP;			\
	return self;						\
}								\
vector *vector##_from(type *src, u64 length) {			\
	vector *self = malloc(sizeof(vector));			\
	assert(self);						\
	self->data = malloc(sizeof(type) * length);		\
	memcpy(self->data, src, length);			\
	self->length = length;					\
	self->capacity = length;				\
	return self;						\
}								\
void destroy_##vector(vector *self) {				\
	free(self->data);					\
	self->data = NULL;					\
	free(self);						\
}								\
type vector##_get(vector *self, u64 index) {			\
	assert(index < self->length);				\
	return self->data[index];				\
}								\
void vector##_set(vector *self, u64 index, type value) {	\
	assert(index < self->length);				\
	self->data[index] = value;				\
}								\
void vector##_add(vector *self, type value) {			\
	if(self->length == self->capacity) {			\
		self->data  = _expand(				\
			self->data,				\
			self->length,				\
			sizeof(type)				\
		);						\
		self->capacity *= 2;				\
	}							\
	self->data[self->length] = value;			\
	self->length++;						\
}								\
void vector##_remove(vector *self, u64 index) {			\
	assert(index < self->length);				\
	type *removed = &self->data[index];			\
	if(index == self->length - 1) {				\
		memset(removed, 0, sizeof(type));		\
	}							\
	else {							\
		type *pushed = &self->data[index+1];		\
		memmove(					\
			removed,				\
			pushed,					\
			(self->length-index-1) * sizeof(type)	\
		);						\
	}							\
	self->length--;						\
	if(							\
		self->length <= self->capacity / 4 &&		\
		self->capacity >= INITIAL_VECTOR_CAP * 2	\
	) {							\
		self->data = _shrink(				\
			self->data,				\
			self->capacity,				\
			sizeof(type)				\
		);						\
		self->capacity /= 2;				\
	}							\
}

IMPL_VECTOR(i8, i8_Vector)
IMPL_VECTOR(i16, i16_Vector)
IMPL_VECTOR(i32, i32_Vector)
IMPL_VECTOR(i64, i64_Vector)

IMPL_VECTOR(u8, u8_Vector)
IMPL_VECTOR(u16, u16_Vector)
IMPL_VECTOR(u32, u32_Vector)
IMPL_VECTOR(u64, u64_Vector)

IMPL_VECTOR(f32, f32_Vector)
IMPL_VECTOR(f64, f64_Vector)

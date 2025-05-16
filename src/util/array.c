#include "array.h"

#define IMPL_ARRAY(type, name) 					\
	name *create_##name(u64 capacity) { 			\
		name *self = malloc(sizeof(name)); 		\
		assert(self); 					\
		self->data = calloc(capacity, sizeof(type)); 	\
		self->capacity = capacity; 			\
		self->length = 0; 				\
		return self; 					\
	} 							\
	name *name##_from(type *src, u64 length) { 		\
		name *self = malloc(sizeof(name));		\
		assert(self);					\
		self->data = malloc(sizeof(type) * length);	\
		memcpy(self->data, src, sizeof(type) * length);	\
		self->length = length;				\
		self->capacity = length;			\
		return self;					\
	}							\
	void destroy_##name(name *self) {			\
		free(self->data);				\
		self->data = NULL;				\
		free(self);					\
	}							\
	type name##_get(name *self, u64 index) {		\
		assert(index < self->length);			\
		return self->data[index];			\
	}							\
	void name##_set(name *self, u64 index, type value) {	\
		assert(index < self->length);			\
		self->data[index] = value;			\
	}							\
	void name##_add(name *self, type value) {		\
		assert(self->length < self->capacity);		\
		self->data[self->length] = value;		\
		self->length++;					\
	}							\
	void name##_remove(name *self, u64 index) {		\
		assert(index < self->length);			\
		type *removed = &self->data[index];		\
		if(index == self->length - 1) {			\
			memset(					\
				removed,			\
				0,				\
				sizeof(type)			\
			);					\
		}						\
		else {						\
			type *pushed = &self->data[index+1];	\
			memmove(				\
				removed,			\
				pushed,				\
				(self->length-index-1)		\
				* sizeof(type)			\
			);					\
		}						\
		self->length--;					\
	}							

IMPL_ARRAY(i8, i8_Array)
IMPL_ARRAY(i16, i16_Array)
IMPL_ARRAY(i32, i32_Array)
IMPL_ARRAY(i64, i64_Array)

IMPL_ARRAY(u8, u8_Array)
IMPL_ARRAY(u16, u16_Array)
IMPL_ARRAY(u32, u32_Array)
IMPL_ARRAY(u64, u64_Array)

IMPL_ARRAY(f32, f32_Array)
IMPL_ARRAY(f64, f64_Array)

#ifndef VECTOR_H
#define VECTOR_H

#include "util.h"

#define DECL_VECTOR(type, vector) 	\
	typedef struct {		\
		type *data;		\
		u64 length, capacity;	\
	} vector;			\
	\
	vector *create_##vector(void);				\
	vector *vector##_from(type *src, u64 length);		\
	void destroy_##vector(vector *self);			\
	type vector##_get(vector *self, u64 index);		\
	void vector##_set(vector *self, u64 index, type value);	\
	void vector##_add(vector *self, type value);		\
	void vector##_remove(vector *self, u64 index);		\

DECL_VECTOR(i8, i8_Vector)
DECL_VECTOR(i16, i16_Vector)
DECL_VECTOR(i32, i32_Vector)
DECL_VECTOR(i64, i64_Vector)

DECL_VECTOR(u8, u8_Vector)
DECL_VECTOR(u16, u16_Vector)
DECL_VECTOR(u32, u32_Vector)
DECL_VECTOR(u64, u64_Vector)

DECL_VECTOR(f32, f32_Vector)
DECL_VECTOR(f64, f64_Vector)

#define create_vector(type)	 			\
	create_##type##_Vector()			\

#define vector_from(src, cap) _Generic(			\
	(src),						\
	i8* : 	i8_Vector_from,				\
	i16*: 	i16_Vector_from,			\
	i32*: 	i32_Vector_from,			\
	i64*: 	i64_Vector_from,			\
	u8* : 	u8_Vector_from,				\
	u16*: 	u16_Vector_from,			\
	u32*: 	u32_Vector_from,			\
	u64*: 	u64_Vector_from,			\
	f32*: 	f32_Vector_from,			\
	f64*: 	f64_Vector_from				\
)(src, cap)

#define destroy_vector(vector) _Generic(		\
	vector,						\
	i8_Vector* : 	destroy_i8_Vector,		\
	i16_Vector*: 	destroy_i16_Vector,		\
	i32_Vector*: 	destroy_i32_Vector,		\
	i64_Vector*: 	destroy_i64_Vector,		\
	u8_Vector* : 	destroy_u8_Vector,		\
	u16_Vector*: 	destroy_u16_Vector,		\
	u32_Vector*: 	destroy_u32_Vector,		\
	u64_Vector*: 	destroy_u64_Vector,		\
	f32_Vector*: 	destroy_f32_Vector,		\
	f64_Vector*: 	destroy_f64_Vector		\
)(vector)

#define vector_get(list, index) _Generic( 		\
	list,						\
	i8_Vector* : 	i8_Vector_get,			\
	i16_Vector*: 	i16_Vector_get,			\
	i32_Vector*: 	i32_Vector_get,			\
	i64_Vector*: 	i64_Vector_get,			\
	u8_Vector* : 	u8_Vector_get,			\
	u16_Vector*: 	u16_Vector_get,			\
	u32_Vector*: 	u32_Vector_get,			\
	u64_Vector*: 	u64_Vector_get,			\
	f32_Vector*: 	f32_Vector_get,			\
	f64_Vector*: 	f64_Vector_get			\
)(list, index)

#define vector_set(list, index, value) _Generic( 	\
	list,						\
	i8_Vector* : 	i8_Vector_set,			\
	i16_Vector*: 	i16_Vector_set,			\
	i32_Vector*: 	i32_Vector_set,			\
	i64_Vector*: 	i64_Vector_set,			\
	u8_Vector* : 	u8_Vector_set,			\
	u16_Vector*: 	u16_Vector_set,			\
	u32_Vector*: 	u32_Vector_set,			\
	u64_Vector*: 	u64_Vector_set,			\
	f32_Vector*: 	f32_Vector_set,			\
	f64_Vector*: 	f64_Vector_set			\
)(list, index, value)

#define vector_add(list, value) _Generic(		\
	list,						\
	i8_Vector* : 	i8_Vector_add,			\
	i16_Vector*: 	i16_Vector_add,			\
	i32_Vector*: 	i32_Vector_add,			\
	i64_Vector*: 	i64_Vector_add,			\
	u8_Vector* : 	u8_Vector_add,			\
	u16_Vector*: 	u16_Vector_add,			\
	u32_Vector*: 	u32_Vector_add,			\
	u64_Vector*: 	u64_Vector_add,			\
	f32_Vector*: 	f32_Vector_add,			\
	f64_Vector*: 	f64_Vector_add			\
)(list, value)

#define vector_remove(list, index) _Generic(		\
	list,						\
	i8_Vector* : 	i8_Vector_remove,		\
	i16_Vector*: 	i16_Vector_remove,		\
	i32_Vector*: 	i32_Vector_remove,		\
	i64_Vector*: 	i64_Vector_remove,		\
	u8_Vector* : 	u8_Vector_remove,		\
	u16_Vector*: 	u16_Vector_remove,		\
	u32_Vector*: 	u32_Vector_remove,		\
	u64_Vector*: 	u64_Vector_remove,		\
	f32_Vector*: 	f32_Vector_remove,		\
	f64_Vector*: 	f64_Vector_remove		\
)(list, index)

#endif // VECTOR_H

#ifndef ARRAY_H
#define ARRAY_H

#include "util.h"

#define DECL_ARRAY(type, name) 		\
	typedef struct { 		\
		type *data; 		\
		u64 length, capacity;	\
	} name;				\
	\
	name *create_##name(u64 capacity); 			\
	name *name##_from(type *src, u64 length); 		\
	void destroy_##name(name *self); 			\
	type name##_get(name *self, u64 index); 		\
	void name##_set(name *self, u64 index, type value); 	\
	void name##_add(name *self, type value); 		\
	void name##_remove(name *self, u64 index);		

DECL_ARRAY(i8, i8_Array)
DECL_ARRAY(i16, i16_Array)
DECL_ARRAY(i32, i32_Array)
DECL_ARRAY(i64, i64_Array)

DECL_ARRAY(u8, u8_Array)
DECL_ARRAY(u16, u16_Array)
DECL_ARRAY(u32, u32_Array)
DECL_ARRAY(u64, u64_Array)

DECL_ARRAY(f32, f32_Array)
DECL_ARRAY(f64, f64_Array)

#define create_array(type, cap) 			\
	create_##type##_Array(cap)			\

#define array_from(src, cap) _Generic(			\
	(src),						\
	i8* : 	i8_Array_from,				\
	i16*: 	i16_Array_from,				\
	i32*: 	i32_Array_from,				\
	i64*: 	i64_Array_from,				\
	u8* : 	u8_Array_from,				\
	u16*: 	u16_Array_from,				\
	u32*: 	u32_Array_from,				\
	u64*: 	u64_Array_from,				\
	f32*: 	f32_Array_from,				\
	f64*: 	f64_Array_from				\
)(src, cap)

#define destroy_array(array) _Generic(			\
	array,						\
	i8_Array* : 	destroy_i8_Array,		\
	i16_Array*: 	destroy_i16_Array,		\
	i32_Array*: 	destroy_i32_Array,		\
	i64_Array*: 	destroy_i64_Array,		\
	u8_Array* : 	destroy_u8_Array,		\
	u16_Array*: 	destroy_u16_Array,		\
	u32_Array*: 	destroy_u32_Array,		\
	u64_Array*: 	destroy_u64_Array,		\
	f32_Array*: 	destroy_f32_Array,		\
	f64_Array*: 	destroy_f64_Array		\
)(array)

#define array_get(list, index) _Generic( 		\
	list,						\
	i8_Array* : 	i8_Array_get,			\
	i16_Array*: 	i16_Array_get,			\
	i32_Array*: 	i32_Array_get,			\
	i64_Array*: 	i64_Array_get,			\
	u8_Array* : 	u8_Array_get,			\
	u16_Array*: 	u16_Array_get,			\
	u32_Array*: 	u32_Array_get,			\
	u64_Array*: 	u64_Array_get,			\
	f32_Array*: 	f32_Array_get,			\
	f64_Array*: 	f64_Array_get			\
)(list, index);

#define array_set(list, index, value) _Generic( 	\
	list,						\
	i8_Array* : 	i8_Array_set,			\
	i16_Array*: 	i16_Array_set,			\
	i32_Array*: 	i32_Array_set,			\
	i64_Array*: 	i64_Array_set,			\
	u8_Array* : 	u8_Array_set,			\
	u16_Array*: 	u16_Array_set,			\
	u32_Array*: 	u32_Array_set,			\
	u64_Array*: 	u64_Array_set,			\
	f32_Array*: 	f32_Array_set,			\
	f64_Array*: 	f64_Array_set			\
)(list, index, value)

#define array_add(list, value) _Generic(		\
	list,						\
	i8_Array* : 	i8_Array_add,			\
	i16_Array*: 	i16_Array_add,			\
	i32_Array*: 	i32_Array_add,			\
	i64_Array*: 	i64_Array_add,			\
	u8_Array* : 	u8_Array_add,			\
	u16_Array*: 	u16_Array_add,			\
	u32_Array*: 	u32_Array_add,			\
	u64_Array*: 	u64_Array_add,			\
	f32_Array*: 	f32_Array_add,			\
	f64_Array*: 	f64_Array_add			\
)(list, value)

#define array_remove(list, index) _Generic(		\
	list,						\
	i8_Array* : 	i8_Array_remove,		\
	i16_Array*: 	i16_Array_remove,		\
	i32_Array*: 	i32_Array_remove,		\
	i64_Array*: 	i64_Array_remove,		\
	u8_Array* : 	u8_Array_remove,		\
	u16_Array*: 	u16_Array_remove,		\
	u32_Array*: 	u32_Array_remove,		\
	u64_Array*: 	u64_Array_remove,		\
	f32_Array*: 	f32_Array_remove,		\
	f64_Array*: 	f64_Array_remove		\
)(list, index)

#endif // ARRAY_H

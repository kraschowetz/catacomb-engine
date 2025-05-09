#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "util.h"

#define ARRAYLIST_INITIAL_CAP 8

typedef struct {
	void *data;
	u64 length, capacity;
	u64 type_size;
} ArrayList;

ArrayList *create_arraylist(u64 type_size);
void destroy_arraylist(ArrayList *self);

void arraylist_add(ArrayList *self, void *restrict value);
void arraylist_remove(ArrayList *self, u64 index);
void arraylist_set(ArrayList *self, u64 index, void *value);
void *arraylist_get(ArrayList *self, u64 index);

#define arraylist_addraw(self, val) {\
	typeof(val) _temp = val; \
	arraylist_add(self, &_temp); \
}

#endif // ARRAYLIST_H

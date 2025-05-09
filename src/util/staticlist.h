#ifndef STATICLIST_H
#define STATICLIST_H

#include "util.h"

typedef struct {
	void *data;
	u64 type_size;
	u64 length, capacity;
} StaticList;

StaticList *create_staticlist(u64 type_size, u64 capacity);
void destroy_staticlist(StaticList *self);

void staticlist_add(StaticList *self, void *value);
void staticlist_set(StaticList *self, u64 index, void *value);
void *staticlist_get(StaticList *self, u64 index);
void staticlist_remove(StaticList *self, u64 index);

#endif // STATICLIST_H

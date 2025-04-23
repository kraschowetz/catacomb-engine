#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

#define STD_HASHMAP_CAP 16

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct HashEntry {
	const char *key;
	void *value;
} HashEntry;

typedef struct HashMap {
	HashEntry *entries;
	size_t capacity, lenght;
} HashMap;

HashMap *create_hash_table(void);
void destroy_hash_table(HashMap *self);

void *hash_table_get(const HashMap *restrict self, const char *key);
const char *hash_table_set(HashMap *restrict self, const char *key, void *value);

typedef struct HashMapIter {
	const char *key;
	void *val;
	HashMap *table;
	size_t index;
} HashMapIter;

HashMapIter hash_table_iter(HashMap *self);
bool hash_table_next(HashMapIter *self);

#endif

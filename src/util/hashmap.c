#include "hashmap.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

static uint64_t hash_key(const char *key) {
	uint64_t hash = FNV_OFFSET;
	for(const char *p = key; *p; p++) {
		hash ^= (unsigned char)*p;
		hash *= FNV_PRIME;
	}
	return hash;
}

// isso é um pecado
static const char *
hashmap_set_entry(
	HashEntry *entries, size_t capacity, 
	const char *key, void *value, size_t *lenght
) {
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

	while (entries[index].key != NULL) {
        	if (strcmp(key, entries[index].key) == 0) {
        		entries[index].value = value;
        		return entries[index].key;
        	}

		index++;
		if(index > capacity) {
			index = 0;
		}
	}

	if(lenght) {
		key = strdup(key);
		if(key == NULL) {
			return NULL;
		}
		(*lenght)++;
	}
	entries[index].key = (char*)key;
	entries[index].value = value;
	return key;
}

static bool hashmap_expand(HashMap *self) {
	size_t new_capacity = self->capacity * 2;

	if (new_capacity < self->capacity) {
        	return false;  // vericar ulong overflow
	}
	
	HashEntry *new_entries = calloc(new_capacity, sizeof(HashEntry));
	if(!new_entries) {
		return false;
	}
	
	memcpy(new_entries, self->entries, sizeof(HashEntry) * self->capacity);

	free(self->entries);
	self->entries = new_entries;
	self->capacity = new_capacity;

	return true;
}

HashMap *create_hashmap(void) {
	HashMap *self = malloc(sizeof(HashMap));
	if(!self) {
		return NULL;
	}

	self->lenght = 0;
	self->capacity = STD_HASHMAP_CAP;

	self->entries = calloc(self->capacity, sizeof(HashEntry));
	if(!self->entries) {
		free(self);
		return NULL;
	}

	return self;
}

void destroy_hashmap(HashMap *self) {
	for(size_t i = 0; i < self->capacity; i++) {
		free((void*)self->entries[i].key);
	}

	free(self->entries);
	free(self);
}

void *hashmap_get(const HashMap *restrict self, const char *key) {
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(self->capacity - 1));

	while(self->entries[index].key != NULL) {
		if(strcmp(key, self->entries[index].key) == 0) {
			return self->entries[index].value;
		}
		index++;
		if(index >= self->capacity) {
			index = 0;
		}
	}
	return NULL;
}

const char *hashmap_set(HashMap *restrict self, const char *key, void *value) {
	if(value == NULL) {
		return NULL;
	}

	if(self->lenght >= self->capacity-2) {
		if(!hashmap_expand(self)) {
			return NULL;
		}
	}

	return hashmap_set_entry(
		self->entries,
		self->capacity, 
		key,
		value, 
		&self->lenght
	);
}

HashMapIter hashmap_iter(HashMap *self) {
	HashMapIter iter;
	iter.table = self;
	iter.index = 0;
	return iter;
}

bool hashmap_next(HashMapIter *self) {
	HashMap *table = self->table;
	while(self->index < table->capacity) {
		size_t i = self->index;
		self->index++;
		
		if(table->entries[i].key) {
			HashEntry entry = table->entries[i];
			self->key = entry.key;
			self->val = entry.value;
			return true;
		}
	}

	return false;
}

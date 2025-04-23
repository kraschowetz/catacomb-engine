#include "strutil.h"

#include "string.h"
#include <stdlib.h>

char *strdup(const char *str) {
	size_t len = strlen(str) + 1;
	char *dup = malloc(len);
	if(!dup) {
		return NULL;
	}
	strcpy(dup, str);
	return dup;
}

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h> 	// IWYU pragma: export
#include <stdlib.h>	// IWYU pragma: export
#include <string.h>	// IWYU pragma: export
#include <stdbool.h>	// IWYU pragma: export
#include <assert.h>	// IWYU pragma: export
#include "types.h"	// IWYU pragma: export
#include "fmath.h"	// IWYU pragma: export
#include "strutil.h"	// IWYU pragma: export

#define THROW_ERRF(_err, ...) fprintf(stderr, _err, __VA_ARGS__); exit(1)
#define THROW_ERR(_err) fprintf(stderr, _err); exit(1)

#endif

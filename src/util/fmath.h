#ifndef FMATH_H
#define FMATH_H

#include "types.h"	// IWYU pragma: export
#include <math.h>	// IWYU pragma: export
#include <cglm/cglm.h>

#define PI ((f32)(GLM_PI))
#define PI_2 ((f32)(GLM_PI_2))
#define TAU (2 * PI)

#define COSF(x) ((f32)(cos(x)))
#define SINF(x) ((f32)(sin(x)))

#define DEG2RAD(x) ((x) * (__typeof__(x))(PI / 180.0f))
#define RAD2DEG(x) ((x) * (__typeof__(x))(180.0f / PI))

#define MIN(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a < _b ? _a : _b; \
})

#define MAX(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a > _b ? _a : _b; \
})

#define CLAMP(x, mn, mx) ({\
	__typeof__(x) _x = (x); \
	__typeof__(mn) _min = (mn); \
	__typeof__(mx) _max = (mx); \
	MAX(_min, MIN(_max, _x)); \
})

#endif

#ifndef FMATH_H
#define FMATH_H

#include "types.h"
#include <math.h>
#include <cglm/cglm.h>

#define PI ((f32)(GLM_PI))
#define PI_2 ((f32)(GLM_PI_2))
#define TAU (2 * PI)

#define COSF(x) ((f32)(cos(x)))
#define SINF(x) ((f32)(sin(x)))

#define DEG2RAD(x) ((x) * (typeof(x))(PI / 180.0f))
#define RAD2DEG(x) ((x) * (typeof(x))(180.0f / PI))

#define MIN(a, b) ({ \
	typeof(a) _a = (a); \
	typeof(b) _b = (b); \
	_a < _b ? _a : _b; \
})

#define MAX(a, b) ({ \
	typeof(a) _a = (a); \
	typeof(b) _b = (b); \
	_a > _b ? _a : _b; \
})

#define CLAMP(x, mn, mx) ({\
	typeof(x) _x = (x); \
	typeof(mn) _min = (mn); \
	typeof(mx) _max = (mx); \
	MAX(_min, MIN(_max, _x)); \
})

#endif

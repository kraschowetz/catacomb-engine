#ifndef C_TRANSFORM_H
#define C_TRANSFORM_H

#include "../util/util.h"
#include <cglm/struct.h>

typedef struct C_Transform {
	vec2s position, scale;
	f32 rotation;
} C_Transform;

#endif // C_TRANSFORM_H

#ifndef C_TRANSFORM_H
#define C_TRANSFORM_H

#include <cglm/struct.h>
#include "../util/util.h"

typedef struct C_Transform {
	vec2s position, scale;
	f32 rotation;
} C_Transform;

#endif // C_TRANSFORM_H

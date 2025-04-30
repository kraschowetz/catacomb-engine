#ifndef CAMERA_H
#define CAMERA_H

#include "../util/util.h"
#include <cglm/struct.h>

typedef struct {
	mat4s view, projection;
} ViewProjection;

typedef struct {
	ViewProjection view_proj;
	vec3s position, direction, up, right;
	f32 pitch, yaw, fov, aspect, near, far;
} PerspectiveCamera;

typedef struct {
	ViewProjection view_proj;
	vec2s position, min, max;
} OrthoCamera;

PerspectiveCamera *create_perspective_camera(f32 fov);
void destroy_perspective_camera(PerspectiveCamera *self);
void perspective_camera_update(PerspectiveCamera *self);

OrthoCamera *create_ortho_camera(vec2s min, vec2s max);
void destroy_ortho_camera(OrthoCamera *self);
void ortho_camera_update(OrthoCamera *self);

#endif

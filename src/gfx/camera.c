#include "camera.h"
#include "../core/state.h"

PerspectiveCamera *create_perspective_camera(f32 fov) {
	PerspectiveCamera *self = calloc(1, sizeof(PerspectiveCamera));
	self->fov = fov;
	self->aspect = game_state.window.aspect;
	self->near = 0.01f;
	self->far = 1000.0f;

	perspective_camera_update(self);

	return self;
}

void destroy_perspective_camera(PerspectiveCamera *self) {
	free(self);
}

void perspective_camera_update(PerspectiveCamera *self) {
	self->pitch = CLAMP(self->pitch, -PI_2, PI_2);
	self->yaw = (self->yaw < 0 ? TAU : 0.0f) + fmodf(self->yaw, TAU);
	
	self->direction = (vec3s){{
		cosf(self->pitch) * sinf(self->yaw),
		sinf(self->pitch),
		cosf(self->pitch) * cosf(self->yaw)
	}};
	glms_vec3_normalize(self->direction);

	self->right = glms_vec3_cross((vec3s){{0.0f, 1.0f, 0.0f}}, self->direction);
	self->up = glms_vec3_cross(self->direction, self->right);

	self->view_proj.view = glms_lookat(
		self->position,
		glms_vec3_add(self->position, self->direction),
		self->up
	);
	self->view_proj.projection = glms_perspective(
		self->fov,
		self->aspect,
		self->near,
		self->far
	);
}

OrthoCamera *create_ortho_camera(vec2s min, vec2s max) {
	OrthoCamera *self = calloc(1, sizeof(OrthoCamera));

	self->min = min;
	self->max = max;

	ortho_camera_update(self);

	return self;
}

void destroy_ortho_camera(OrthoCamera *self) {
	free(self);
}

void ortho_camera_update(OrthoCamera *self) {
	self->view_proj.view = glms_mat4_identity();
	self->view_proj.projection = glms_ortho(
		self->min.x,
		self->max.x,
		self->min.y,
		self->max.y,
		-10.0f,
		10.f
	);
}

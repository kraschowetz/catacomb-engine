#include "c_sprite.h"
#include "c_transform.h"
#include "ecs.h"
#include "ecs_components.h"
#include <cglm/types-struct.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb/stb_image.h"

static void _render(C_Sprite *self, Entity entity) {

	C_Transform *transform = (C_Transform*) ecs_get(entity, C_TRANSFORM);
	
	if(!transform) {
		// render sprtie at screen center;	
		return;
	}

	// render sprite;
}

static void _init(C_Sprite *self, Entity entity) {
	C_Transform *transform = (C_Transform*) ecs_get(entity, C_TRANSFORM);

	i32 w, h, ch;
	u8 *raw_bytes = stbi_load("res/sprite.png", &w, &h, &ch, STBI_rgb_alpha);

	// put this in a better place, like, create_renderer() or smth
	stbi_set_flip_vertically_on_load(true);	

	glGenTextures(1, &self->gl_handle);

	glBindTexture(GL_TEXTURE_2D, self->gl_handle);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		w,
		h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		raw_bytes
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	self->vao = create_vao();
	vao_bind(&self->vao);
	self->position = create_vbo(GL_ARRAY_BUFFER);
	self->uv = create_vbo(GL_ARRAY_BUFFER);
	
	vec3s _size;
	vec3s _position; 
	if(transform) {
		_size = (vec3s){{transform->scale.x / 2, transform->scale.y / 2, 1.f}};
		_position = (vec3s){{transform->position.x, transform->position.y, 0.f}};
	}
	else {
		_size = glms_vec3_one();
		_position = glms_vec3_zero();
	}
	
	f32 raw_pos[12] = {
		_position.x - _size.x,
		_position.y - _size.y,
		_position.z,
		_position.x + _size.x,
		_position.y - _size.y,
		_position.z,
		_position.x + _size.x,
		_position.y + _size.y,
		_position.z,
		_position.x - _size.x,
		_position.y + _size.y,
		_position.z

	};
	
	f32 uv[8] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	vbo_buffer(
		&self->position,
		(void*) raw_pos,
		0,
		sizeof(f32) * 12
	);
	vao_attr(&self->vao, &self->position, 0, 3, GL_FLOAT, 0, 0);
	
	vbo_buffer(
		&self->uv,
		(void*) uv,
		0,
		sizeof(f32) * 8
	);
	vao_attr(&self->vao, &self->uv, 1, 2, GL_FLOAT, 0, 0);
	
	stbi_image_free(raw_bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

}

static void _destroy(C_Sprite *self, Entity entity) {
	destroy_vao(&self->vao);
	destroy_vbo(&self->position);
	destroy_vbo(&self->uv);
	glDeleteTextures(1, &self->gl_handle);
}

void c_sprite_init(struct ECS *ecs) {
	ECS_REGISTER(
		C_SPRITE,
		C_Sprite,
		ecs,
		((ECS_System) {
			.init = (ECS_Subscriber) _init,
			.destroy = NULL, // create destructor
			.render = (ECS_Subscriber) _render,
			.tick = NULL
		})
	);
}

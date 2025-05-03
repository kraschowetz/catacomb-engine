#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in quad_id;

#define MAX_QUADS 8

uniform vec2 base_pos[MAX_QUADS];
uniform vec2 scale[MAX_QUADS];
uniform vec2 tex_pos[MAX_QUADS];
uniform vec2 tex_scale[MAX_QUADS];

out vec2 tex_coords;

void main() {
	vec2 _scale = position.xy * scale[quad_id];
	vec3 _new_pos = base_pos[quad_id] + vec3(_scale, 0.0);

	gl_Position = vec4(_new_pos, 1.0);

	vec2 _tex_scale = position * tex_scale[quad_id];
	tex_coords = tex_pos[quad_id] + _tex_scale;
}

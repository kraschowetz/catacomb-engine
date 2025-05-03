#version 410 core

in vec2 v_uv;

out vec4 color;

uniform sampler2D u_sprite_atlas;

void main() {
	color = texture(
		u_sprite_atlas,
		v_uv
	);
}

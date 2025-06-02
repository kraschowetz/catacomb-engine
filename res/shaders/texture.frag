#version 410 core

in vec2 v_uv;

uniform sampler2D u_sprite_atlas;

out vec4 color;

void main() {
	color = texture(
		u_sprite_atlas,
		v_uv
	);

	if(color == vec4(0)) {
		discard;
	}
}

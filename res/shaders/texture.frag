#version 410 core

in vec2 uv_coord;

uniform sampler2D tex;

out vec4 color;

void main() {
	color = texture(tex, uv_coord);
}

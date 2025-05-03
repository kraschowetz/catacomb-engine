#version 410

in vec2 tex_coords;
out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture2D(sampler, tex_coords.xy);

	if(color == vec4(0.0)) {
		discard;
	}
}



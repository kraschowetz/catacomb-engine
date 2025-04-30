#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;

uniform mat4 u_model;
uniform mat4 u_perspective;
uniform mat4 u_view;

out vec3 vertex_color;

void main() {
	vec4 _position = u_perspective * u_view * u_model * vec4(position, 1.0f);

	gl_Position = vec4(
		_position.x,
		_position.y,
		_position.z,
		_position.w
	);

	vertex_color = color;
}

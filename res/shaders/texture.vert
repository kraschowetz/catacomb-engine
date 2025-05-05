#version 410 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_perspective;
uniform mat4 u_view;

out vec2 v_uv;

void main() {
	vec4 _position = u_perspective * u_view * u_model * vec4(a_pos, 1.0f);

	gl_Position = vec4(
		_position.x,
		_position.y,
		_position.z,
		_position.w
	);

	v_uv = a_uv;
}

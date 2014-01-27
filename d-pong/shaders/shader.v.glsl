#version 110

uniform float timer;

attribute vec4 position;

void main()
{
	const mat4 projection = mat4(
		vec4(3.0/4.0, 0.0, 0.0, 0.0),
		vec4(    0.0, 1.0, 0.0, 0.0),
		vec4(    0.0, 0.0, 0.5, 0.5),
		vec4(    0.0, 0.0, 0.0, 1.0)
	);

	mat4 rotation = mat4(
		vec4(1.0,         0.0,         0.0, 0.0),
		vec4(0.0,  cos(timer),  sin(timer), 0.0),
		vec4(0.0, -sin(timer),  cos(timer), 0.0),
		vec4(0.0,         0.0,         0.0, 1.0)
	);

	mat4 scale_aspect = mat4(
		vec4(4.0/3.0, 0.0, 0.0, 0.0),
		vec4(    0.0, 1.0, 0.0, 0.0),
		vec4(    0.0, 0.0, 1.0, 0.0),
		vec4(    0.0, 0.0, 0.0, 1.0)
	);

	mat4 scale_size = mat4(
		vec4(0.5, 0.0, 0.0, 0.0),
		vec4(    0.0, 0.5, 0.0, 0.0),
		vec4(    0.0, 0.0, 0.5, 0.0),
		vec4(    0.0, 0.0, 0.0, 1.0)
	);

	mat4 move = mat4(
		vec4(1.0, 0.0, 0.0, 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(cos(timer),   0.0,   3.0+sin(timer),   1.0)
	);

	gl_Position = projection * move * rotation * position;
}

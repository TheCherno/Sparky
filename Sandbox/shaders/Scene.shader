#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec3 normal;
	vec2 uv;
	vec3 color;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	vs_out.position = ml_matrix * position;
	vs_out.normal = normal;
	vs_out.uv = uv;
	vs_out.color = vec3(normal * 0.5 + 0.5);
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in DATA
{
	vec4 position;
	vec3 normal;
	vec2 uv;
	vec3 color;
} fs_in;

uniform vec4 u_Color;

void main()
{
	vec3 n = fs_in.normal / 2.0 + 0.5;
	color = vec4(fs_in.color, 1.0);
};
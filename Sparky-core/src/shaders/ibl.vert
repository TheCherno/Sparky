#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in float tid;
layout (location = 4) in vec4 color;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec3 position;
	vec3 camera_pos;
	vec3 normal;
	vec2 uv;
	float tid;
	vec4 color;

	vec3 reflectDir;
	vec3 reflectNormal;
} vs_out;

void main()
{
	mat4 mvp = pr_matrix * vw_matrix * ml_matrix;
	mat4 normalMatrix = transpose(inverse(mvp));
	gl_Position = mvp * position;

	vs_out.position = vec3(ml_matrix * position);
	vs_out.camera_pos = vec3(vw_matrix * vec4(0.0, 0.0, 0.0, -1.0));
	vs_out.normal = vec3(ml_matrix * vec4(normal, 0));
	vs_out.uv = uv;
	vs_out.tid = tid;
	vs_out.color = color;

	vs_out.reflectNormal = normalize(mat3(normalMatrix) * normal);
	vs_out.reflectDir = reflect(gl_Position.xyz, vs_out.reflectNormal);
}
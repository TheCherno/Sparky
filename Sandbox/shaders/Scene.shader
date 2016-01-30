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
	vs_out.normal = mat3(ml_matrix) * normal;
	vs_out.uv = uv;
	vs_out.color = vec3(normal * 0.5 + 0.5);
}

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
uniform vec3 u_LightPosition;
uniform float u_LightAttenuation;
uniform vec4 u_LightColor;

void main()
{
	vec3 light = u_LightPosition - vec3(fs_in.position);
	float face = dot(normalize(light), normalize(fs_in.normal));
	float intensity = (u_LightAttenuation / length(light)) * max(face, 0.0);
	intensity = max(intensity, 0.1);
	color = vec4(fs_in.color * intensity, 1.0);
}
#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out vec4 pos;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	pos = ml_matrix * position;
}
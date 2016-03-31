#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_DepthMatrix;
uniform mat4 ml_matrix;

void main()
{
	gl_Position = u_DepthMatrix * ml_matrix * position;
}

#shader fragment
#version 330 core

layout(location = 0) out float depth;

void main()
{
	depth = gl_FragCoord.z;
}
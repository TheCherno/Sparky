R"(
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	vs_out.position = vec3(ml_matrix * position);
	vs_out.normal = vec3(ml_matrix * vec4(normal, 0.0));
	vs_out.uv = uv;
};

#shader fragment
#version 330 core

layout(location = 0) out vec3 position;   
layout(location = 1) out vec3 diffuse;     
layout(location = 2) out vec3 normal;     
layout(location = 3) out vec3 uv;    

uniform sampler2D u_Texture;

in DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

void main()
{
	position = fs_in.position;
	diffuse = vec3(texture(u_Texture, fs_in.uv));
	normal = normalize(fs_in.normal);
	uv = vec3(fs_in.uv, 0.0);
};
)"
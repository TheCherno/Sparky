R"(
#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec2 mask_uv;
layout (location = 3) in float tid;
layout (location = 4) in float mid;
layout (location = 5) in vec4 color;

uniform mat4 pr_matrix;

out DATA
{
	vec2 uv;
} vs_out;

void main()
{
	gl_Position = pr_matrix * position;
	vs_out.uv = uv;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

uniform sampler2D u_Texture;

in DATA
{
	vec2 uv;
} fs_in;

void main()
{
	color = texture(u_Texture, fs_in.uv);
};
)"
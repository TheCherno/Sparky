R"(
#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float tid;
layout (location = 3) in vec4 color;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	vs_out.position = ml_matrix * position;
	vs_out.uv = uv;
	vs_out.tid = tid;
	vs_out.color = color;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;

in DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	float intensity = 1.0 / length(fs_in.position.xy - light_pos);
	vec4 texColor = fs_in.color;
	if (fs_in.tid > 0.0)
	{
		int32 tid = int32(fs_in.tid - 0.5);
		texColor = fs_in.color * texture(textures[tid], fs_in.uv);
	}
	color = texColor * intensity;
};
)"
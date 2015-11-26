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
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

uniform mat4 mask_matrix;

out DATA
{
	vec4 position;
	vec2 uv;
	vec2 mask_uv;
	float tid;
	float mid;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	vs_out.position = ml_matrix * position;
	vs_out.uv = uv;
	vs_out.tid = tid;
	vs_out.mid = mid;
	vs_out.color = color;
	vs_out.mask_uv = mask_uv;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in DATA
{
	vec4 position;
	vec2 uv;
	vec2 mask_uv;
	float tid;
	float mid;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	vec4 texColor = fs_in.color;
	vec4 maskColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (fs_in.tid > 0.0)
	{
		int tid = int(fs_in.tid - 0.5);
		texColor = fs_in.color * texture(textures[tid], fs_in.uv);
	}
	if (fs_in.mid > 0.0)
	{
		int mid = int(fs_in.mid - 0.5);
		maskColor = texture(textures[mid], fs_in.mask_uv);
	}
	color = texColor * maskColor; // vec4(1.0 - maskColor.x, 1.0 - maskColor.y, 1.0 - maskColor.z, 1.0);
};
)"
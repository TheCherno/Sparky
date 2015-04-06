#version 120

//layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;

/*in DATA
{
	vec4 position;
	vec4 color;
} fs_in;*/

uniform sampler2D tex;

void main()
{
	//float intensity = 1.0 / length(fs_in.position.xy - light_pos);
	// color = colour * intensity;
	gl_FragColor = texture2D(tex, gl_TexCoord[0].st); //fs_in.color * intensity;
}
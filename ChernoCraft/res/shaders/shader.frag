#version 330 core

layout (location = 0) out vec4 color;
uniform vec3 block_color = vec3(1.0, 1.0, 1.0);

uniform vec3 player_pos;
uniform float light_on;
uniform float selected;
uniform sampler2D tex_id;

vec3 light = vec3(2.0, 48.0, 2.0);

in DATA
{
    vec3 position;
    vec3 normal;
    vec2 tc;
} fs_in;

void main()
{
    vec3 to_light = player_pos - fs_in.position;
    float intensity = dot(normalize(to_light), normalize(fs_in.normal));
    intensity = min(10.0 / length(to_light) * max(intensity, 0.0), 2.0);
    vec3 col = vec3(texture(tex_id, fs_in.tc));
    if (light_on == 1.0) col *= intensity;
    color = vec4(col + 0.05, 1.0);
    if (selected == 1.0) color += 0.3;
}
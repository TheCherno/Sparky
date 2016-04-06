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

layout(location = 0) out vec4 color;

in DATA
{
	vec4 position;
	vec3 normal;
	vec2 uv;
	vec3 color;
} fs_in;

// TODO: Replace with struct
uniform vec3 u_LightPosition;
uniform vec4 u_LightColor;
uniform float u_LightIntensity;

uniform float u_Glossiness;
uniform vec3 u_CameraPos;

uniform mat4 vw_matrix = mat4(1.0);

struct Light
{
	vec3 position;
	vec4 color;
	float intensity;
	vec3 lightVector;
};

struct Surface
{
	float glossiness;
};

float Blinn(Light light, Surface surface, vec3 eye)
{
	vec3 direction = light.lightVector;
	float face = max(dot(direction, fs_in.normal), 0.0);
	float result = face;
	return result;
}

float Phong(Light light, Surface surface, vec3 eye)
{
	vec3 r = reflect(light.lightVector, fs_in.normal);
	float reflecty = pow(max(dot(r, eye), 0.0), surface.glossiness);
	return reflecty;
}

float Diffuse(Light light, Surface surface, vec3 eye)
{
	return Blinn(light, surface, eye);
}

float Specular(Light light, Surface surface, vec3 eye)
{
	return Phong(light, surface, eye);
}

// TODO: Windowing
float Attenuate(Light light)
{
	vec3 direction = light.position - vec3(fs_in.position);
	return u_LightIntensity * 1.0 / (dot(direction, direction) + 0.01); // TODO: 4pi
}

void main()
{
	fs_in.normal = normalize(fs_in.normal);

	Light light;
	light.position = u_LightPosition;
	light.color = u_LightColor;
	light.intensity = Attenuate(light);
	light.lightVector = normalize(light.position - vec3(fs_in.position));

	vec3 eye = normalize(fs_in.position.xyz - u_CameraPos);

	Surface surface;
	surface.glossiness = u_Glossiness;

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	// TODO: Multiple lights
	for (int i = 0; i < 1; i++)
	{
		diffuse += Diffuse(light, surface, eye) * light.color * light.intensity;
		specular += Specular(light, surface, eye) * light.color * light.intensity;
	}
	color = vec4(fs_in.color, 1.0);
	color = color * diffuse + specular;
	color.a = 1.0;
}
//
//	PBR Shader
//

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float3 cameraPosition : CAMERA_POSITION;
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 color : COLOR;
	float4 shadowCoord : SHADOW_POSITION;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 sys_ProjectionMatrix;
	float4x4 sys_ViewMatrix;
	float4x4 sys_ModelMatrix;
	float3	 sys_CameraPosition;
	// float4x4 sys_DepthBiasMatrix;
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)sys_ModelMatrix;

	VSOutput output;
	output.position = mul(input.position, sys_ModelMatrix);
	output.positionCS = mul(output.position, mul(sys_ViewMatrix, sys_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.binormal = mul(input.binormal, wsTransform);
	output.tangent = mul(input.tangent, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); // output.shadowCoord = mul(output.position, depthBias);

	output.cameraPosition = sys_CameraPosition;

	return output;
}

struct Light
{
	float4 color;
	float3 position;
	float p0;
	float3 direction;
	float p1;
	float3 lightVector;
	float intensity;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
	float3 binormal;
	float3 tangent;
};

cbuffer PSSystemUniforms : register(b0)
{
	Light sys_Light;
};

cbuffer PSUniforms : register(b1)
{
	// PBR Static Inputs
	float4 u_AlbedoColor;
	float3 u_SpecularColor;
	float u_GlossColor;
	float3 u_NormalColor;

	// PBR Modes
	float u_UsingAlbedoMap;
	float u_UsingSpecularMap;
	float u_UsingNormalMap;
	float u_UsingGlossMap;
	float p1;
}

SamplerState AnisoClamp
{
	Filter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

// PBR Map Inputs
Texture2D u_AlbedoMap : register(t0);
SamplerState u_AlbedoSampler : register(s0);
Texture2D u_SpecularMap : register(t1);
SamplerState u_SpecularSampler : register(s1);
Texture2D u_GlossMap : register(t2);
SamplerState u_GlossSampler : register(s2);
Texture2D u_NormalMap : register(t3);
SamplerState u_NormalSampler : register(s3);

Texture2D u_PreintegratedFG : register(t4);
SamplerState u_PreintegratedFGSampler : register(s4);

TextureCube u_EnvironmentMap : register(t5);
SamplerState u_EnvironmentMapSampler : register(s5);

struct Material
{
	float4 albedo;
	float3 specular;
	float roughness;
	float3 normal;
};

#define PI 3.1415926535897932384626433832795f
#define GAMMA 2.2f

/*float2 poissonDisk[16] = float2[](
	float2(-0.94201624, -0.39906216),
	float2(0.94558609, -0.76890725),
	float2(-0.094184101, -0.92938870),
	float2(0.34495938, 0.29387760),
	float2(-0.91588581, 0.45771432),
	float2(-0.81544232, -0.87912464),
	float2(-0.38277543, 0.27676845),
	float2(0.97484398, 0.75648379),
	float2(0.44323325, -0.97511554),
	float2(0.53742981, -0.47373420),
	float2(-0.26496911, -0.41893023),
	float2(0.79197514, 0.19090188),
	float2(-0.24188840, 0.99706507),
	float2(-0.81409955, 0.91437590),
	float2(0.19984126, 0.78641367),
	float2(0.14383161, -0.14100790)
	);*/

float4 GammaCorrectTexture(Texture2D t, SamplerState s, float2 uv)
{
	float4 samp = t.Sample(s, uv);
	return float4(pow(samp.rgb, GAMMA), samp.a);
}

float3 GammaCorrectTextureRGB(Texture2D t, SamplerState s, float2 uv)
{
	float4 samp = t.Sample(s, uv);
	return float3(pow(samp.rgb, GAMMA));
}

float4 GetAlbedo(Attributes attributes)
{
	return (1.0f - u_UsingAlbedoMap) * u_AlbedoColor + u_UsingAlbedoMap * GammaCorrectTexture(u_AlbedoMap, u_AlbedoSampler, attributes.uv);
}

float3 GetSpecular(Attributes attributes)
{
	return (1.0f - u_UsingSpecularMap) * u_SpecularColor + u_UsingSpecularMap * GammaCorrectTextureRGB(u_SpecularMap, u_SpecularSampler, attributes.uv);
}

float GetGloss(Attributes attributes)
{
	return (1.0f - u_UsingGlossMap) * u_GlossColor + u_UsingGlossMap * GammaCorrectTextureRGB(u_GlossMap, u_GlossSampler, attributes.uv).r;
}

float GetRoughness(Attributes attributes)
{
	return 1.0f - GetGloss(attributes);
}

float3 FinalGamma(float3 color)
{
	return pow(color, 1.0f / GAMMA);
}

float FresnelSchlick(float f0, float fd90, float view)
{
	return f0 + (fd90 - f0) * pow(max(1.0f - view, 0.1f), 5.0f);
}

float Disney(Attributes attributes, Light light, Material material, float3 eye)
{
	float3 halfVector = normalize(light.lightVector + eye);

	float NdotL = saturate(dot(attributes.normal, light.lightVector));
	float LdotH = saturate(dot(light.lightVector, halfVector));
	float NdotV = saturate(dot(attributes.normal, eye));

	float energyBias = lerp(0.0f, 0.5f, material.roughness);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, material.roughness);
	float fd90 = energyBias + 2.0f * (LdotH * LdotH) * material.roughness;
	float f0 = 1.0f;

	float lightScatter = FresnelSchlick(f0, fd90, NdotL);
	float viewScatter = FresnelSchlick(f0, fd90, NdotV);

	return lightScatter * viewScatter * energyFactor;
}

float3 GGX(Attributes attributes, Light light, Material material, float3 eye)
{
	float3 h = normalize(light.lightVector + eye);
	float NdotH = saturate(dot(attributes.normal, h));

	float rough2 = max(material.roughness * material.roughness, 2.0e-3f); // capped so spec highlights don't disappear
	float rough4 = rough2 * rough2;

	float d = (NdotH * rough4 - NdotH) * NdotH + 1.0f;
	float D = rough4 / (PI * (d * d));

	// Fresnel
	float3 reflectivity = material.specular;
	float fresnel = 1.0;
	float NdotL = saturate(dot(attributes.normal, light.lightVector));
	float LdotH = saturate(dot(light.lightVector, h));
	float NdotV = saturate(dot(attributes.normal, eye));
	float3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473f * LdotH - 6.98316f) * LdotH);

	// geometric / visibility
	float k = rough2 * 0.5f;
	float G_SmithL = NdotL * (1.0f - k) + k;
	float G_SmithV = NdotV * (1.0f - k) + k;
	float G = 0.25f / (G_SmithL * G_SmithV);

	return G * D * F;
}

float3 RadianceIBLIntegration(float NdotV, float roughness, float3 specular)
{
	float2 preintegratedFG = u_PreintegratedFG.Sample(AnisoClamp, float2(roughness, 1.0f - NdotV)).rg;
	return specular * preintegratedFG.r + preintegratedFG.g;
}

float3 IBL(Attributes attributes, Light light, Material material, float3 eye)
{
	// Note: Currently this function assumes a cube texture resolution of 1024x1024
	float NdotV = max(dot(attributes.normal, eye), 0.0f);

	float3 reflectionVector = normalize(reflect(-eye, attributes.normal));
	float smoothness = 1.0f - material.roughness;
	float mipLevel = (1.0f - smoothness * smoothness) * 10.0f;
	float4 cs = u_EnvironmentMap.SampleLevel(u_EnvironmentMapSampler, reflectionVector, mipLevel);
	float3 result = pow(cs.xyz, GAMMA) * RadianceIBLIntegration(NdotV, material.roughness, material.specular);

	float3 diffuseDominantDirection = attributes.normal;
	float diffuseLowMip = 9.6;
	float3 diffuseImageLighting = u_EnvironmentMap.SampleLevel(u_EnvironmentMapSampler, diffuseDominantDirection, diffuseLowMip).rgb;
	diffuseImageLighting = pow(diffuseImageLighting, GAMMA);

	return result + diffuseImageLighting * material.albedo.rgb;
}

float Diffuse(Attributes attributes, Light light, Material material, float3 eye)
{
	return Disney(attributes, light, material, eye);
}

float3 Specular(Attributes attributes, Light light, Material material, float3 eye)
{
	return GGX(attributes, light, material, eye);
}

// TODO: Windowing
float Attenuate(Attributes attributes, Light light)
{
	float3 direction = light.position - float3(attributes.position);
	return light.intensity * 1.0 / (dot(direction, direction) + 0.01); // TODO: 4pi
}

float random(float3 seed, int i)
{
	float4 seed4 = float4(seed, i);
	float dot_product = dot(seed4, float4(12.9898, 78.233, 45.164, 94.673));
	return frac(sin(dot_product) * 43758.5453);
}

float3 NormalMap(Attributes attributes)
{
	float3x3 toWorld = float3x3(attributes.binormal, attributes.tangent, attributes.normal);
	float3 normalMap = u_NormalMap.Sample(u_NormalSampler, attributes.uv).rgb * 2.0 - 1.0;
	normalMap = mul(normalMap.rgb, toWorld);
	normalMap = normalize(normalMap);
	return normalMap;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	Attributes attributes;
	attributes.position = input.position;
	attributes.uv = input.uv;
	attributes.normal = normalize(input.normal);
	attributes.binormal = normalize(input.binormal);
	attributes.tangent = normalize(input.tangent);

	if (u_UsingNormalMap > 0.0)
		attributes.normal = NormalMap(attributes);

	float3 eye = normalize(input.cameraPosition - attributes.position);

	Light light = sys_Light;
	light.intensity = light.intensity; // Attenuate(light);
	light.lightVector = light.direction; // normalize(light.position - float3(fs_in.position));

	Material material;
	material.albedo = GetAlbedo(attributes);
	material.specular = GetSpecular(attributes);
	material.roughness = GetRoughness(attributes);

	float4 diffuse = float4(0, 0, 0, 0);
	float3 specular = float3(0, 0, 0);

	// TODO: Multiple lights
	for (int i = 0; i < 1; i++)
	{
		float NdotL = saturate(dot(attributes.normal, light.lightVector));

		// Diffuse Calculation
		diffuse += NdotL * Diffuse(attributes, light, material, eye) * light.color * light.intensity;
		// Specular Calculation
		specular += NdotL * Specular(attributes, light, material, eye) * light.color.xyz * light.intensity;
		light.intensity /= 2.0;
		light.lightVector = -light.lightVector;
	}
	// Shadows
	float bias = 0.005;
	float visibility = 1.0;
	for (int i = 0; i < 1; i++)
	{
		// int index = int(16.0 * random(floor(attributes.position.xyz * 1000.0), i)) % 16;
		// visibility -= (1.0 / 4.0) * (1.0 - texture(u_ShadowMap, float3(fs_in.shadowCoord.xy + poissonDisk[index] / 700.0, (fs_in.shadowCoord.z - bias) / fs_in.shadowCoord.w)));
	}

	float3 finalColor = material.albedo.rgb * diffuse.rgb * visibility + (specular + IBL(attributes, light, material, eye)) * visibility;
	finalColor = FinalGamma(finalColor);
	return float4(finalColor, material.albedo.a);
}
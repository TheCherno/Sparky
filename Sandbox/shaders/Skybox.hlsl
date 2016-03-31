
struct VSInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 sys_ProjectionMatrix;
	float4x4 sys_ViewMatrix;
	float4x4 sys_ModelMatrix;
	float3	 sys_CameraPosition;
};

cbuffer VSUniforms : register(b1)
{
	float4x4 invViewProjMatrix;
}

VSOutput VSMain(in VSInput input)
{
	VSOutput output;
	float4 pos = input.position;
	pos.z = 1.0f;
	output.positionCS = pos;
	output.position = mul(pos, invViewProjMatrix);
	output.uv = input.uv;
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

cbuffer PSSystemUniforms : register(b0)
{
	Light sys_Light;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
};

TextureCube u_EnvironmentMap : register(t0);
SamplerState u_EnvironmentMapSampler : register(s0);

float4 PSMain(in VSOutput input) : SV_TARGET
{
	Attributes attributes;
	attributes.position = input.position;
	attributes.uv = input.uv;

	return u_EnvironmentMap.SampleLevel(u_EnvironmentMapSampler, input.position, 4);
}
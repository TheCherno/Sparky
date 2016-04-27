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
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output;
	output.position = mul(input.position, sys_ProjectionMatrix);
	output.positionCS = input.position;
	output.uv = input.uv;
	return output;
}

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
};

Texture2D u_ScreenTexture : register(t0);
SamplerState u_ScreenTextureSampler : register(s0);

float4 PSMain(in VSOutput input) : SV_TARGET
{
	Attributes attributes;
	attributes.position = input.position;
	attributes.uv = input.uv;

	float2 direction = float2(1.0f, 2.0f);
	float resolution = 512.0f;
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float2 off1 = float2(1.411764705882353f, 1.411764705882353f) * direction;
	float2 off2 = float2(3.2941176470588234f, 3.2941176470588234f) * direction;
	float2 off3 = float2(5.176470588235294f, 5.176470588235294f) * direction;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv) * 0.1964825501511404;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv + (off1 / resolution)) * 0.2969069646728344;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv - (off1 / resolution)) * 0.2969069646728344;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv + (off2 / resolution)) * 0.09447039785044732;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv - (off2 / resolution)) * 0.09447039785044732;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv + (off3 / resolution)) * 0.010381362401148057;
	color += u_ScreenTexture.Sample(u_ScreenTextureSampler, attributes.uv - (off3 / resolution)) * 0.010381362401148057;
	return color;
}
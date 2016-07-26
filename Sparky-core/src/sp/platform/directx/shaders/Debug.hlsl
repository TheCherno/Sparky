R"(

struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float4 position : POSITION;
	float4 color : COLOR;
};

cbuffer VSUniforms : register(b0)
{
	float4x4 sys_ProjectionMatrix;
	float4x4 sys_ViewMatrix;
}

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.positionCS = mul(input.position, mul(sys_ViewMatrix, sys_ProjectionMatrix));
	output.position = output.positionCS;
	output.color = input.color;
	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	return input.color;
};

)"
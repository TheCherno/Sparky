
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
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 sys_ProjectionMatrix;
	float4x4 sys_ViewMatrix;
	float4x4 sys_ModelMatrix;
	float4x4 sys_LightViewMatrix;
	float3	 sys_CameraPosition;
};

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

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	float4 pos = mul(sys_ModelMatrix, input.position);
	output.position = mul(sys_LightViewMatrix, pos);

	// Store the position value in a second input value for depth value calculations.
	output.depthPosition = output.position;
	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}

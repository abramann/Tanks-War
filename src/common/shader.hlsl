cbuffer cbPerObject
{
	float4x4 gcWVP;
};

Texture2D gTexture;

SamplerState gSamplerState;

struct VS_Input
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

struct PS_Input
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

PS_Input VS_Start(VS_Input input)
{
	PS_Input output;
	output.position = mul(input.position, gcWVP);
	output.uv = input.uv;
	return output;
}

float4 PS_Start(PS_Input input) : SV_TARGET
{
	return gTexture.Sample(gSamplerState,input.uv);
}
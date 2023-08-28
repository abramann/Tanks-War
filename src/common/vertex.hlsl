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

cbuffer cbPerObject
{
	float4x4 gcWVP;
};

PS_Input main(VS_Input input)
{
	PS_Input output;
	output.position = mul(input.position, gcWVP);
	output.uv = input.uv;
	return output;
}
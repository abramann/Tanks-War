// vertex.hlsl
// Author: abramann

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

float4x4 translateMatrix(float3 translation)
{
	return float4x4(
		float4(1.0f, 0.0f, 0.0f, 0.0f),
		float4(0.0f, 1.0f, 0.0f, 0.0f),
		float4(0.0f, 0.0f, 1.0f, 0.0f),
		float4(translation.x, translation.y, translation.z, 1.0f)
		);
}

float4x4 scallingMatrix(float3 scalling)
{
	return float4x4(
		float4(scalling[0], 0.0f, 0.0f, 0.0f),
		float4(0.0f, scalling[1], 0.0f, 0.0f),
		float4(0.0f, 0.0f, scalling[2], 0.0f),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
		);
}

float4x4 identityMatrix4x4()
{
	return float4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

float4x4 rotateXMatrix(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	return float4x4(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
		);
}

float4x4 rotateYMatrix(float angle)
{
	float s = sin(angle);
	float c = cos(angle);
	return float4x4(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1
		);
}

float4x4 rotateZMatrix(float angle)
{
	float s = sin(angle);
	float c = cos(angle);
	return float4x4(
		c, s, 0.0f, 0.0f,
		-s, c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0, 0.0,
		0.0f, 0.0, 0.0, 1.0
		);
}

cbuffer cbPerFrame : register(b0)
{
	float4x4 g_worldViewMatrix;
};

cbuffer cbPerObject : register(b1)
{
	float3 g_position, g_scalling, g_rotate, extra;
};

PS_Input main(VS_Input input)
{
	float4x4 objectMatrix = identityMatrix4x4();
	objectMatrix = mul(objectMatrix, scallingMatrix(g_scalling));
	for (int i = 2; i >= 0; i--)
		objectMatrix = mul(objectMatrix, rotateZMatrix(g_rotate[i]));

	objectMatrix = mul(objectMatrix,translateMatrix(g_position));
	objectMatrix = mul(objectMatrix, transpose(g_worldViewMatrix));

	PS_Input output;
	output.position = mul(input.position, objectMatrix);
	output.uv = input.uv;
	return output;
}
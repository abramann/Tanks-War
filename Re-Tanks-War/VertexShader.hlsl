// vertex.hlsl
// Author: abramann


#define UNDEFINED_POSITION 0xFFFF

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

float4x4 scalingMatrix(float3 scaling)
{
	return float4x4(
		float4(scaling[0], 0.0f, 0.0f, 0.0f),
		float4(0.0f, scaling[1], 0.0f, 0.0f),
		float4(0.0f, 0.0f, scaling[2], 0.0f),
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
	float4 g_position;
	float4 g_scaling;
	float4 g_rotate;
	float4 rotateCenter;
};

float4 rotateVertex(float4 vertex, float angle, float3 origin)
{
	float translatedToOriginX = vertex[0] - origin[0];
	float translatedToOriginY = vertex[2] - origin[2];

	float rotatedX = translatedToOriginX * cos(angle) - translatedToOriginY * sin(angle);
	float rotatedY = translatedToOriginX * sin(angle) + translatedToOriginY * cos(angle);

	float reverseTranslatedX = rotatedX + origin[0];
	float reverseTranslatedY = rotatedY + origin[1];

	float4 finalPosition = { reverseTranslatedX, reverseTranslatedY, vertex[2], vertex[3]};

	return finalPosition;
}

PS_Input main(VS_Input input)
{
	PS_Input output;
	output.uv = input.uv;

	if (g_position[0] == UNDEFINED_POSITION) // Is not relative to camera object
	{
		output.position = input.position;
	}
	else
	{
		/*float4x4 objectMatrix = identityMatrix4x4();
		float4x4 rotateCenterN = translateMatrix(rotateCenter * -1);
		objectMatrix = mul(objectMatrix, rotateCenterN);
		for (int i = 2; i >= 0; i--)
			objectMatrix = mul(objectMatrix, rotateZMatrix(g_rotate[i]));

		objectMatrix = mul(objectMatrix, translateMatrix(rotateCenter));
		objectMatrix = mul(objectMatrix, scalingMatrix(g_scaling));

		objectMatrix = mul(objectMatrix, translateMatrix(g_position));
		objectMatrix = mul(objectMatrix, transpose(g_worldViewMatrix));

		output.position = mul(input.position, objectMatrix);
		*/

		float4x4 objectMatrix = identityMatrix4x4();
		float4x4 rotateCenterN = translateMatrix(rotateCenter * -1);
		objectMatrix = mul(objectMatrix, rotateCenterN);
		for (int i = 2; i >= 0; i--)
			objectMatrix = mul(objectMatrix, rotateZMatrix(g_rotate[i]));

		objectMatrix = mul(objectMatrix, translateMatrix(rotateCenter));
		objectMatrix = mul(objectMatrix, scalingMatrix(g_scaling));

		objectMatrix = mul(objectMatrix, translateMatrix(g_position));
		objectMatrix = mul(objectMatrix, transpose(g_worldViewMatrix));

		output.position = mul(input.position, objectMatrix);
	}
	return output;
}
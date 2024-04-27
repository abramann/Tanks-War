
struct Input
{
	float2 position : POSITION;
	float2 uv : TEXCOORD;
};

struct Output
{
	float4 position: SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer cbPerFrame : register(b0)
{
	float4x4 g_worldViewMatrix;
};

cbuffer cbPerDraw : register(b1)
{
	uint objRelativeToCamera;
	uint padding;
	float2 objPosition;
	float2 objScalling;
	float2 objRotate;
	float2 objRotateCenter;
};

float4x4 translateMatrix(float3 translation);

float4x4 scallingMatrix(float3 scalling);

float4x4 identityMatrix4x4();

float4x4 rotateXMatrix(float angle);

float4x4 rotateYMatrix(float angle);

float4x4 rotateZMatrix(float angle);

Output main(Input input)
{
	float2 position = input.position;
	 if (objRelativeToCamera != 0)
	{
		/*float4x4 objectMatrix = identityMatrix4x4();
		objectMatrix = mul(objectMatrix, scallingMatrix(objScalling));
		float4x4 rotateCenterN = translateMatrix(objRotateCenter * -1);
		objectMatrix = mul(objectMatrix, rotateCenterN);
		for (int i = 2; i >= 0; i--)
			objectMatrix = mul(objectMatrix, rotateZMatrix(g_rotate[i]));

		objectMatrix = mul(objectMatrix, translateMatrix(rotateCenter));
		objectMatrix = mul(objectMatrix, translateMatrix(objPosition));
		objectMatrix = mul(objectMatrix, transpose(g_worldViewMatrix));

		position = mul(position, objectMatrix);*/
		//position[0] = position[1] = position[2] = 0;
	 }
	Output output;
	output.position[0] = position[0];
	output.position[1] = position[1];
	output.position[2] = 0;
	output.position[3] = 1;
	output.uv[0] = input.uv[0];
	output.uv[1] = input.uv[1];
	return output;
}

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
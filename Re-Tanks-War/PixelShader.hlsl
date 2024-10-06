
struct PS_Input
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D gTexture;
SamplerState gSamplerState;

cbuffer cbPerDraw : register(b0)
{
	uint color;
}

float4 main(PS_Input input) : SV_TARGET
{
	float r = (color >> 24 & 0xFF) / 255.0f;
	float g = (color >> 16 & 0xFF) / 255.0f;
	float b = (color >> 8 & 0xFF) / 255.0f;
	float a = (color & 0xFF) / 255.0f;

	float4 finalColor = { r, g, b, a };
	finalColor *= gTexture.Sample(gSamplerState, input.uv);

	if(finalColor[3] == 0)
		discard;

	return finalColor;
}
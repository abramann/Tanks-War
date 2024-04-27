
struct PS_Input
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D gTexture;
SamplerState gSamplerState;

cbuffer cbColor : register(b0)
{
	uint color;
}

float4 main(PS_Input input) : SV_TARGET
{ 
	float4 rgba;
	rgba[0] = (color >> 24 & 0xFF) / 255.0f; // red
	rgba[1] = (color >> 16 & 0xFF) / 255.0f; // green
	rgba[2] = (color >> 8 & 0xFF) / 255.0f; // blue
	rgba[3] = color & 0xFF;	// alpha
	if (input.uv[0] == 0 && input.uv[1] == 0)
		{
			return rgba;
		}
	return gTexture.Sample(gSamplerState, input.uv);
}
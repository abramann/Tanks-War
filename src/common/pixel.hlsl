#include "vertex.hlsl"

Texture2D gTexture;
SamplerState gSamplerState;

float4 main(PS_Input input) : SV_TARGET
{

	float4 f = {0.1,0.9,0.9,1.0f};
    return gTexture.Sample(gSamplerState, input.uv)*f;
}
#include "vertex.hlsl"

Texture2D gTexture;
SamplerState gSamplerState;

float4 main(PS_Input input) : SV_TARGET
{
    return gTexture.Sample(gSamplerState, input.uv);
}
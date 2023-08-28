#include "global.hlsli"


struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

SamplerState splr : register(s0);

float4 main(VSOut psIn) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
    float3 color = CubeMapTexture.Sample(skyBoxSampler, normalize(psIn.WorldPos)).rgb;
		
    return float4(color, 1.0);
}
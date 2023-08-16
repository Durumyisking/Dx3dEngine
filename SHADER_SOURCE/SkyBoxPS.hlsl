#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

//SamplerState splr : register(s0);

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(VSOut psIn) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
    float3 color = CubeMapTexture.Sample(skyBoxSampler, normalize(psIn.WorldPos)).rgb;
		
    return float4(color, 1.0);
}
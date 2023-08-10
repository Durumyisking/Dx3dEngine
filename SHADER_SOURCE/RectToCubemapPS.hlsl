#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float3 LocalPos : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

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
    float2 uv = SampleSphericalMap(normalize(psIn.LocalPos));
    float3 colour = CubeMapTexture.Sample(linearSampler, uv).rgb;
		
    return float4(colour, 1.0);
}
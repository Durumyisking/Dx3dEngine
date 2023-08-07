#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float4 WorldPos : POSITION;
};

float2 SampleSphericalMap(float3 v)
{
    float2 invAtan = float2(0.1591, 0.3183);
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


float4 main(VSOut vsIn) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(vsIn.WorldPos.xyz)); // make sure to normalize localPos
    float3 color = skyboxTexture.Sample(linearSampler, normalize(vsIn.WorldPos.rgb)).rgb;

    //return tex.Sample(splr, input.worldPos);
    return float4(color, 1.0f);
}
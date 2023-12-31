#include "global.hlsli"

struct VSIn
{
    float4 Position : Position;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};


VSOut main(VSIn vsIn)
{
    VSOut vsOut;
    
    vsOut.WorldPos = vsIn.Position.xyz;
    vsOut.Position = mul(float4(vsIn.Position.xyz, 1.0), cubemapMat);
    vsOut.Position.z = vsOut.Position.w;

    return vsOut;
}


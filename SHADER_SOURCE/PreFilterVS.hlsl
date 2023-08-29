#include "global.hlsli"

struct VSIn
{
    float4 Position : Position;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
    float3 ViewPos : Position1;
};


VSOut main(VSIn vsIn)
{
    VSOut vsOut;
    
    float4 worldPosition = mul(float4(vsIn.Position.xyz, 1.0f), world);
    float4 viewPosition = mul(worldPosition, view);
    vsOut.WorldPos = vsIn.Position;
    vsOut.Position = mul(float4(vsIn.Position.xyz, 1.0), cubemapMat);
    vsOut.Position.z = vsOut.Position.w;
    vsOut.ViewPos = viewPosition;

    return vsOut;
}


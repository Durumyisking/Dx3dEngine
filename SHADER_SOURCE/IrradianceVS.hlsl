#include "global.hlsli"

struct VSIn
{
    float4 Position : Position;
};

struct VSOut
{
    float4 ProjPosition : SV_Position;
    float4 WorldPosition : Position;
};


VSOut main(VSIn vsIn)
{
    VSOut vsOut;
    
    float4 worldPosition = mul(vsIn.Position, world);

    vsOut.WorldPosition = worldPosition;
    vsOut.ProjPosition = mul(float4(vsIn.Position.xyz, 1.0), cubemapMat);
    vsOut.ProjPosition.z = vsOut.ProjPosition.w;

    return vsOut;
}


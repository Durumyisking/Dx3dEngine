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
    
    float4 viewPos = mul(float4(vsIn.Position.xyz, 0.0f), view);

    vsOut.WorldPosition = vsIn.Position;
    vsOut.ProjPosition = mul(float4(vsIn.Position.xyz, 1.0), cubemapMat);
    vsOut.ProjPosition.z = vsOut.ProjPosition.w;

    return vsOut;
}


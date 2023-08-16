#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.f;
    
    vsIn.Position.w = 1.f;
    
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection); // cubemap proj °öÇØ¾ßÇÒµí?
    
    vsOut.WorldPos = worldPosition.xyz;
    vsOut.Position = mul(worldPosition, cubemapMat);
    vsOut.Position.z = vsOut.Position.w;
    
    return vsOut;
}

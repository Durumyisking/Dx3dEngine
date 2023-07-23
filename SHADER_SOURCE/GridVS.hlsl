#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
    
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    float2 WorldPos : POSITION;
};



VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.f;
        
    float4 worldPosition = mul(vsIn.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projectionPosition = mul(viewPosition, projection);
       
    float2 resolution = cbxy1;
    float3 cameraPosition = cbxyz1;
    
    vsOut.Pos = projectionPosition;
    vsOut.UV = vsIn.UV;
    vsOut.WorldPos = vsIn.Pos.xz * resolution.xy + cameraPosition.xz;
    
    return vsOut;
}
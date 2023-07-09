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



VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
        
    float4 worldPosition = mul(In.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projectionPosition = mul(viewPosition, projection);
       
    float2 Resolution = cbxy1;
    float3 Camera_position = cbxyz1;
    
    Out.Pos = projectionPosition;
    Out.UV = In.UV;
    Out.WorldPos = In.Pos.xy * Resolution.xy + Camera_position.xy;
    
    return Out;
}
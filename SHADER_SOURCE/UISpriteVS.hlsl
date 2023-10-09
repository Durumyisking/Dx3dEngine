#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    float4 worldPosition = mul(In.Pos, world);
    //float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(worldPosition, projection);
    
    //Out.WorldPos = worldPosition.xyz;
    Out.Pos = ProjPosition;
    //Out.Pos = ProjPosition;
    Out.WorldPos = worldPosition.xyz;
    Out.UV = In.UV;
   
    return Out;
}
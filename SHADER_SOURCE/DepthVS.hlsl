#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    
    float4 BlendID : BLENDINDICES;
    float4 BlendWeight : BLENDWEIGHT;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
};



VSOut main(VSIn vsIn)
{
    VSOut output = (VSOut) 0.0f;
    float4 weights = vsIn.BlendWeight;
    weights.w = 1.f - (weights.x + weights.y + weights.z); 
  
    float4 pos = mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.x].bMatrix) * vsIn.BlendWeight.x;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.y].bMatrix) * vsIn.BlendWeight.y;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.z].bMatrix) * vsIn.BlendWeight.z;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.w].bMatrix) * weights.w;

    pos = vsIn.BlendWeight.x + vsIn.BlendWeight.y + vsIn.BlendWeight.z + vsIn.BlendWeight.w == 0.0f ? vsIn.Position : pos;
    
    
    float4 worldPosition = mul(float4(pos.xyz, 1.f), world);
    float4 viewPosition = mul(worldPosition, lightAttributes[0].view);
    float4 ProjPosition = mul(viewPosition, lightAttributes[0].projection);
    
    output.Position = ProjPosition;
    output.ProjPosition = ProjPosition;
    
    return output;
}
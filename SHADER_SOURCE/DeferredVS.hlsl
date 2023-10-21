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
    float2 UV : TEXCOORD;
    float3 WorldPos : POSITION0;
    float4 WorldViewPos: POSITION1;
    
    float3 WorldTangent : TANGENT;
    float3 WorldNormal : NORMAL;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.f;
        
    float4 weights = vsIn.BlendWeight;
    weights.w = 1.f - (weights.x + weights.y + weights.z);
  
    float4 pos = mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.x].bMatrix) * vsIn.BlendWeight.x;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.y].bMatrix) * vsIn.BlendWeight.y;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.z].bMatrix) * vsIn.BlendWeight.z;
    pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.w].bMatrix) * weights.w;

    pos = vsIn.BlendWeight.x + vsIn.BlendWeight.y + vsIn.BlendWeight.z + vsIn.BlendWeight.w == 0.0f ? vsIn.Position : pos;
    
    
    float4 worldPosition = mul(float4(pos.xyz, 1.f), world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    vsOut.Position = ProjPosition;
    vsOut.UV = vsIn.UV;

    float4 Normal = float4(vsIn.Normal, 0.f);
    Normal = mul(Normal, worldIT);
    Normal = normalize(Normal);
    
    float4 Tangent = float4(vsIn.Tangent, 0.f);
    Tangent = mul(Tangent, world);
    Tangent = normalize(Tangent);

    
    vsOut.WorldPos = worldPosition.xyz;
    vsOut.WorldViewPos = float4(ProjPosition.z / ProjPosition.w, 0.f, 0.f, 0.f);
    vsOut.WorldNormal = Normal.xyz;
    vsOut.WorldTangent = Tangent.xyz;
    
    return vsOut;
}


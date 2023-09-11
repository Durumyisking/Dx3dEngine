#include "global.hlsli"


//StructuredBuffer<BoneMatrix> BonArray : register(t30);

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    
    float4 BlendID : BLENDINDICES;
    float4 BlendWeight : BLENDWEIGHT;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 WorldPos : POSITION;

    float3 WorldTangent : TANGENT;
    float3 WorldNormal : NORMAL;
    float3 WorldBiNormal : BINORMAL;
   
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.f;
    
    
    //float4 weights = vsIn.BlendWeight;
    //weights.w = 1.f - (weights.x + weights.y + weights.z);
  
    float4 pos = mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.x].bMatrix) * vsIn.BlendWeight.x;
    //pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.y].bMatrix) * vsIn.BlendWeight.y;
    //pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.z].bMatrix) * vsIn.BlendWeight.z;
    //pos += mul(vsIn.Position, BonArray[(uint) vsIn.BlendID.w].bMatrix) * weights.w;

    //if (weights.x + weights.y + weights.z + weights.w == 0.0f)
    pos = vsIn.Position;
    
    float4 worldPosition = mul(float4(pos.xyz, 1.f), world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    vsOut.Position = projPosition;
    vsOut.UV = vsIn.UV;

    // 로컬 노말을 뷰변환
    float3 Normal = normalize(mul(float4(vsIn.Normal.xyz, 0.f), world).xyz);
    
    float3 Tangent = normalize(mul(float4(vsIn.Tangent.xyz, 0.f), world).xyz);
    
    float3 biNormal = cross(vsIn.Normal, vsIn.Tangent);
    float3 BiNormal = normalize(mul(float4(biNormal, 0.f), world).xyz);
    
    vsOut.WorldPos = worldPosition.xyz;
    vsOut.WorldNormal = Normal.xyz;
    vsOut.WorldTangent = Tangent.xyz;
    vsOut.WorldBiNormal = BiNormal.xyz;
        
    return vsOut;
}

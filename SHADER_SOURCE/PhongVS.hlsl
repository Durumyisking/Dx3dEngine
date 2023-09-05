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
    float3 ViewPos : POSITION;

    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    float3 ViewBiNormal : BINORMAL;
   
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
    
    float4 worldPosition = mul(pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    vsOut.Position = projPosition;
    vsOut.UV = vsIn.UV;

    // 로컬 노말을 뷰변환
    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.f), world).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.f), view).xyz);
    
    float3 viewTangent = normalize(mul(float4(vsIn.Tangent.xyz, 0.f), world).xyz);
    viewTangent = normalize(mul(float4(viewTangent, 0.f), view).xyz);
    
    float3 biNormal = cross(vsIn.Normal, vsIn.Tangent);
    float3 viewBiNormal = normalize(mul(float4(biNormal, 0.f), world).xyz);
    viewBiNormal = normalize(mul(float4(viewBiNormal, 0.f), view).xyz);
    
    vsOut.ViewPos = worldPosition.xyz;
    vsOut.ViewNormal     = viewNormal.xyz;
    vsOut.ViewTangent    = viewTangent.xyz;
    vsOut.ViewBiNormal   = viewBiNormal.xyz;
        
    return vsOut;
}

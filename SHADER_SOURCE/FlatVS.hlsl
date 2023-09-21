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
    float3 ViewPos : POSITION;

    nointerpolation float3 ViewNormal : NORMAL;
    nointerpolation float3 ViewTangent : TANGENT;
};


VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    vsOut.Position = projPosition;
    vsOut.UV = vsIn.UV;
    
    // 로컬 노말을 뷰변환
    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.0f), worldIT).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.0f), view).xyz);
    
    float3 viewTangent = normalize(mul(float4(vsIn.Tangent.xyz, 0.0f), world).xyz);
    viewTangent = normalize(mul(float4(viewTangent, 0.0f), view).xyz);
    
    vsOut.ViewPos = viewPosition.xyz;
    vsOut.ViewNormal = viewNormal.xyz;
    vsOut.ViewTangent = viewTangent.xyz;
    
    return vsOut;
}
#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
    
    float3 ViewNormal : NORMAL;
    float3 ViewTangent : TANGENT;
    float3 ViewBiNormal : BINORMAL;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    vsOut.Position = ProjPosition;
    vsOut.UV = vsIn.UV;

    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.0f), world).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.0f), view).xyz);
    
    float3 viewTangent = normalize(mul(float4(vsIn.Tangent.xyz, 0.0f), world).xyz);
    viewTangent = normalize(mul(float4(viewTangent, 0.0f), view).xyz);

    float3 biNormal = cross(vsIn.Normal, vsIn.Tangent);
    float3 viewBiNormal = normalize(mul(float4(biNormal.xyz, 0.0f), world).xyz);
    viewBiNormal = normalize(mul(float4(viewBiNormal, 0.0f), view).xyz);
    
    vsOut.ViewPos = viewPosition.xyz;
    vsOut.ViewNormal = viewNormal.xyz;
    vsOut.ViewTangent = viewTangent.xyz;
    vsOut.ViewBiNormal = viewBiNormal.xyz;
    
    return vsOut;
}


#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
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

struct PSOut
{
    float4 Position : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Normal : SV_Target2;
    float4 MRD    : SV_Target3;
};


PSOut main(VSOut vsIn) : SV_Target
{
    PSOut vsOutColor;
    
    float4 albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float3 normal = vsIn.ViewNormal;
    float metallic = 0.04f;
    float roughness = 0.5f;
    
    if (1 == cbbAlbedo)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
    }
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }    
    if (1 == cbbMetallic)
    {
        metallic = TextureMapping_metallic(vsIn.UV);
    }
    if (1 == cbbRoughness)
    {
        roughness = TextureMapping_roughness(vsIn.UV);
    }
    
    vsOutColor.Position = float4(vsIn.ViewPos, 1.0f);
    vsOutColor.Albedo = albedo;
    vsOutColor.Normal = float4(normal, 1.0f);
    vsOutColor.MRD.r = metallic;
    vsOutColor.MRD.g = roughness;
    vsOutColor.MRD.b = 0.f;
    vsOutColor.MRD.w = 1.f;
    
    return vsOutColor;
}
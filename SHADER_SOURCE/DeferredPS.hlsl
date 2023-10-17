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
    float3 WorldPos : POSITION0;
    float4 WorldViewPos : POSITION1;
    
    float3 WorldTangent : TANGENT;
    float3 WorldNormal : NORMAL;
};

struct PSOut
{
    float4 Position : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Normal : SV_Target2;
    float4 MRD    : SV_Target3;
    float DepthColor : SV_Target4;
};


PSOut main(VSOut vsIn) : SV_TARGET
{
    PSOut vsOutColor;
    
    float4 albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float3 normal = vsIn.WorldNormal;
    float metallic = cbMetallic;
    float roughness = cbRoughness;
    
    float pixelToCam = distance(cameraWorldPos.xyz, vsIn.WorldPos);

    albedo = cbbAlbedo ? TextureMapping_albedo(vsIn.UV, pixelToCam) : albedo;
    normal = cbbNormal ? TextureMapping_normal(vsIn.UV, vsIn.WorldTangent, vsIn.WorldNormal, pixelToCam) : normal;
    metallic = cbbMetallic ? TextureMapping_metallic(vsIn.UV, pixelToCam) : cbbMetallic;
    roughness = cbbRoughness ? TextureMapping_roughness(vsIn.UV, pixelToCam) : cbbRoughness;

    
    vsOutColor.Position = float4(vsIn.WorldPos, 1.0f);
    vsOutColor.Albedo = albedo;
    vsOutColor.Normal = float4(normal, 1.0f);
    vsOutColor.Normal.w = 1.f;//vsIn.UV.x;
    vsOutColor.MRD.r = metallic;
    vsOutColor.MRD.g = roughness;
    vsOutColor.MRD.b = 0.f;
    vsOutColor.MRD.w = 1.f;//vsIn.UV.y;
    vsOutColor.DepthColor.r = vsIn.WorldViewPos.r;
    
    return vsOutColor;
}
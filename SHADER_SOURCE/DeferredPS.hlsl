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
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Data : SV_Target3;
};


PSOut main(VSOut vsIn) : SV_Target
{
    PSOut vsOutColor;
    
    float4 objColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float4 normal = float4(vsIn.ViewNormal, 0.f);
    
    if (0 == cbtextureExistence)
    {
        normal.xyz = vsIn.ViewNormal;
    }
    else if (1 == cbtextureExistence)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
        normal.xyz = vsIn.ViewNormal;
    }
    else if (2 == cbtextureExistence)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    else if (3 == cbtextureExistence)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    else if (4 == cbtextureExistence)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    else if (5 == cbtextureExistence)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    
    vsOutColor.Color = objColor;
    vsOutColor.Normal = float4(normal.xyz, 1.0f);
    vsOutColor.Position = float4(vsIn.ViewPos, 1.0f);
    vsOutColor.Data = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return vsOutColor;
}
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
    float4 Normal : SV_Target1;
    float4 Color : SV_Target2;
    float4 Data : SV_Target3;
};


PSOut main(VSOut vsIn) : SV_Target
{
    PSOut vsOutColor;
    
    float4 objColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float3 normal = vsIn.ViewNormal;
    
    if (1 == cbbAlbedo)
    {
        objColor = TextureMapping_albedo(vsIn.UV);
    }
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }    
    
    vsOutColor.Position = float4(vsIn.ViewPos, 1.0f);
    vsOutColor.Normal = float4(normal, 1.0f);
    vsOutColor.Color = objColor;
    vsOutColor.Data = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return vsOutColor;
}
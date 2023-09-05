#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION; // viewspace fragmentPos

    float3 ViewTangent : TANGENT; // view Space tangent
    float3 ViewNormal : NORMAL; // view Space normal
    float3 ViewBiNormal : BINORMAL; // view Space binormal

};

float4 main(VSOut vsIn) : SV_Target
{
    float3 outColor = (float3) 0.f;

    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;

    float4 albedo = (float4) 1.f;
    float3  normal = vsIn.ViewNormal;
    float3  worldnormal = mul(float4(vsIn.ViewNormal.xyz, 0.f), inverseView);
    float   metallic = 0.01f;
    float   roughness = 0.99f;
    float3  A0 = (float3) 1.f;

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
        metallic  = TextureMapping_metallic(vsIn.UV);
    }
    if (1 == cbbRoughness)
    {
        roughness = TextureMapping_roughness(vsIn.UV);
    }
    if (1 == cbbEmissive)
    {
        //normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
   
    
    outColor.xyz = CalculateLightPBR_Direct(vsIn.ViewPos, albedo, normal, metallic, roughness);
        
    return float4(outColor, 1.f);;

}

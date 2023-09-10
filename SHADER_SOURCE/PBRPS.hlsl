#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 WorldPos : POSITION; // Worldspace fragmentPos

    float3 WorldTangent : TANGENT; // World Space tangent
    float3 WorldNormal : NORMAL; // World Space normal
    float3 WorldBiNormal : BINORMAL; // World Space binormal

};

float4 main(VSOut vsIn) : SV_Target
{
    float3 outColor = (float3) 0.f;

    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;
    //float3 albedo = float3(1.0f, 0.6172f, 0.1384f); //GOLD

    float4  albedo = float4(0.25f, 0.25f, 0.25f, 1.f);
    float3  normal = vsIn.WorldNormal;
    float   metallic = cbMetallic; 
    float   roughness = cbRoughness;
    float3  A0 = (float3) 1.f;

    if (1 == cbbAlbedo)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
    }
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.WorldTangent, vsIn.WorldNormal, vsIn.WorldBiNormal);
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
        //normal = TextureMapping_normal(vsIn.UV, vsIn.WorldTangent, vsIn.WorldNormal, vsIn.WorldBiNormal);
    }

    
    outColor.xyz = CalculateLightPBR_Direct(vsIn.WorldPos, albedo, normal, metallic, roughness);
        
    return float4(outColor, 1.f);;

}

#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
  
    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    float3 ViewBiNormal : BINORMAL;
    
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 albedo = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float3 normal = vsIn.ViewNormal;

    //return albedo;
    if (1 == cbbAlbedo)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
    }
    
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    
    LightColor lightColor = (LightColor) 0.0f;
    
    for (uint i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, normal.xyz, i, lightColor);
    }
    
    albedo = CombineLights(albedo, lightColor);
    
    
    if (albedo.w == 0)
        discard;
    
    if (cbxyzw1.w != 0)
    {
        albedo *= cbxyzw1; // 곱할 색        
    }
    
    if (cbxyzw2.w != 0)
    {
        albedo += cbxyzw2; // 더할 색    
    }
        
    
    return albedo;
        
}
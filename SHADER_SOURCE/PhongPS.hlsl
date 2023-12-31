#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
  
    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 albedo = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float3 normal = vsIn.ViewNormal;
    
    float3 worldPos = mul(float4(vsIn.ViewPos, 1.f), inverseWorld).xyz;
    float pixelToCam = distance(cameraWorldPos.xyz, worldPos);

    albedo = cbbAlbedo ? TextureMapping_albedo(vsIn.UV, pixelToCam) : albedo;
    normal = cbbNormal ? TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, pixelToCam) : normal;

    if (1 == cbbAlbedo)
    {
        albedo = TextureMapping_albedo(vsIn.UV, pixelToCam);
    }
    
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, pixelToCam);
    }
    LightColor lightColor = (LightColor) 0.0f;
    
    for (uint i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, normal.xyz, i, lightColor);
    }
    
    albedo = CombineLights(albedo, lightColor);
    
    
    if (albedo.w <= 0.1f)
        discard;
    
    if (cbxyzw1.w != 0.f)
    {
        albedo *= cbxyzw1; // 곱할 색        
    }
    
    if (cbxyzw2.w != 0.f)
    {
        albedo += cbxyzw2; // 더할 색    
    }
        
    
    return albedo;
        
}
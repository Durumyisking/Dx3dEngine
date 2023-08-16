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

    float2 uv = vsIn.UV;
    uv.y += uv.y * 0.5f;
    if (1 == cbbAlbedo)
    {
        albedo = colorTexture.Sample(anisotropicSampler, uv);
    }
    if (1 == cbbNormal)
    {
        normal = TextureMapping_normal(uv, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    
    
    if (albedo.w <= 0.1f)
        discard;
    
    albedo.w = 1.0f;
    
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
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
  
    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    float3 ViewBiNormal : BINORMAL;
    
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float4 normal = (float4) 0.f;
    
    if (1 == cbtextureExistence) // 알베도만 처리
    {
        outColor = colorTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);        
    }
    else if (2 == cbtextureExistence) // 알베도 노말맵 처리
    {
        outColor = colorTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);
        normal = normalTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);
        
        // 노말을 새로 받아와서 뷰변환 되어있지 않다.
        normal.xyz = normalize((normal.xyz * 2.f) - 1.f);
//        normal.xyz = normalize(mul(float4(normal.xyz, 0.0f), world).xyz);
        normal.xyz = normalize(mul(float4(normal.xyz, 0.0f), view).xyz);
    }
    else if (0 == cbtextureExistence) // 텍스처가 없어요
    {
        normal.xyz = vsIn.ViewNormal;
    }

    LightColor lightColor = (LightColor) 0.0f;
    
    for (int i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, normal.xyz, i, lightColor);
    }
    
    outColor.rgb *= lightColor.diffuse.rgb;

    outColor.rgb += lightColor.specular.rgb;
    
    outColor.rgb += outColor.rgb * lightColor.ambient.rgb;
    
    return outColor;
        
}
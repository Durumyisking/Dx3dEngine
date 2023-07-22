#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
  
    nointerpolation float3 ViewTangent : TANGENT;
    nointerpolation float3 ViewNormal : NORMAL;
    nointerpolation float3 ViewBiNormal : BINORMAL;

};


float4 main(VSOut vsIn) : SV_Target
{ 
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float4 normal = (float4) 0.f;
    
    if (1 == cbtextureExistence)
    {
        outColor = colorTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);
    }
    else if (2 == cbtextureExistence)
    {
        outColor = colorTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);
        normal = normalTexture.SampleLevel(anisotropicSampler, vsIn.UV, 0.f);
        
        // 노말을 새로 받아와서 뷰변환 되어있지 않다.
        normal.xyz = normalize((normal.xyz * 2.f) - 1.f);
        
        
        float3x3 matTBN =
        {
            vsIn.ViewTangent,
          vsIn.ViewBiNormal,
          vsIn.ViewNormal,
        };

        normal = normalize(float4(mul(normal.xyz, matTBN), 0.f));
    }
    else
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
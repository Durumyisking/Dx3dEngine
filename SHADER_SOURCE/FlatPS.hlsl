#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
    nointerpolation float3 ViewNormal : NORMAL;
    float intensity : FOG;

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
        normal.xyz = normalize(mul(float4(normal.xyz, 0.0f), world).xyz);
        normal.xyz = normalize(mul(float4(normal.xyz, 0.0f), view).xyz);
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
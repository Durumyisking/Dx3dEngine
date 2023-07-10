#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
    nointerpolation float3 ViewNormal : NORMAL;
    float intensity : FOG;

};


float4 main(VSOut In) : SV_Target
{
    float4 OutColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    LightAttribute lightAttribute = lightAttributes[0];
       
    float3 ViewLightDir = normalize(mul(float4(lightAttribute.direction.xyz, 0.0f), view));
    float intensity = saturate(dot(-ViewLightDir, In.ViewNormal));

    float fSpecPow = 0.0f;
    float3 vViewReflect = normalize(ViewLightDir + 2.0f * dot(-ViewLightDir, In.ViewNormal) * In.ViewNormal);
    
    float3 vEye = normalize(float3(0.0f, 0.0f, 1.0f)); // 시점 벡터는 고정된 값으로 설정
    
    fSpecPow = saturate(dot(-vEye, vViewReflect));
    fSpecPow = pow(fSpecPow, 30);
    
    OutColor.rgb *= lightAttribute.color.diffuse.rgb * intensity;
    OutColor.rgb += lightAttribute.color.specular.rgb * fSpecPow * 0.5f;
    OutColor.rgb += OutColor.xyz * lightAttribute.color.ambient.rgb;
    
    return OutColor;
}
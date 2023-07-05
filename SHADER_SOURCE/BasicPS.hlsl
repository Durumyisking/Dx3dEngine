#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;
    float intensity : FOG;
};

//diffuse
//specular
//ambient

float4 main(VSOut In) : SV_Target
{
    float4 OutColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    LightAttribute lightAttribute = lightAttributes[0];
       
    //광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환
    float3 ViewLightDir = normalize(mul(float4(lightAttribute.direction.xyz, 0.0f), view));
    
    // 방향에 음수를 취하는 이유
    // 빛을 표면에 표시하고 싶기때문 (내적은 방향이 같을수록 1에 가까워짐)
    // 음수를 취하지 않으면 빛의 방향과 normal벡터의 각이 180도가 넘어가는 즉 구 뒤쪽에 빛이 들어감
    float intensity = saturate(dot(-ViewLightDir, In.ViewNormal));
    float fSpecPow = 0.0f;
    
    //뷰 스페이스 상에서 표면의 빛의 세기를 구해야함
    float3 vViewReflect
    = normalize(ViewLightDir + 2.0f * dot(-ViewLightDir, In.ViewNormal) * In.ViewNormal);
    
    // 시점에서 표면을 향하는 벡터
    float3 vEye = normalize(In.ViewPos);
    
    //시선 벡터랑 반사벡터를 내적해서 반사광의 세기를 구한다.
    fSpecPow = saturate(dot(-vEye, vViewReflect));
    
    // fSpectPow의 30승
    fSpecPow = pow(fSpecPow, 30);
    
    // 분산광
    OutColor.rgb *= lightAttribute.color.diffuse.rgb * intensity;

    // 반사광
    OutColor.rgb += lightAttribute.color.specular.rgb * fSpecPow * 0.5f;

    // 주변광
    OutColor.rgb += (OutColor.xyz * lightAttribute.color.ambient.rgb);
    
    //float4 Out = float4(1.0f, 0.0f, 1.0f, 1.0f);
    //float3 LightDir = normalize(globalLightDir);
    //float intensity = saturate(dot(-LightDir, In.Normal));
    
    //Out.xyz = (intensity * OutColor.xyz * globalLightColor)
    //            + (OutColor.xyz * globalLightAmb);
    
    return OutColor;
        
}
#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
};

/*
실질적인 출력 픽셀값 연산을 처리한다.
*/

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = (float4) 1.0f;
    float2 UV = vsIn.Position.xy / float2(1600.0f, 900.0f);
    
    float4 viewPos = positionTarget.Sample(linearSampler, UV);
    
    if (0.0f == viewPos.a)
        discard;
    
    outColor = albedoTarget.Sample(linearSampler, UV);
    
    
    float4 vDiffuse = diffuseLightTarget.Sample(linearSampler, UV);
    float4 vSpecular = specularLightTarget.Sample(linearSampler, UV);
    
    outColor.rgb = (outColor.rgb * vDiffuse.rgb);
    
    
    return outColor;
}
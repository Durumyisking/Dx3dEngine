#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
};


struct PS_OUT
{
    float4 vDiffuse : SV_Target;
    float4 vSpecular : SV_Target1;
};


PS_OUT main(VSOut vsin)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 vViewPos = positionTarget.Sample(linearSampler, vUV);
    
    if (0.f == vViewPos.a)
        discard;
      
    float4 vViewNormal = normalTarget.Sample(linearSampler, vUV);

    LightColor lightcolor = (LightColor) 0.f;
    CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, 0, lightcolor);
    
    output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
    output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
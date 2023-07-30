#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
};

struct PSOut
{
    float4 Diffuse : SV_Target0;
    float4 Specular : SV_Target1;
};


PSOut main(VSOut vsIn)
{
    PSOut outcolor = (PSOut) 0.0f;
    
    float2 UV = vsIn.Position.xy / float2(1600.0f, 900.0f);
    float4 viewPos = positionTarget.Sample(anisotropicSampler, UV);
    
    if (1.0f != viewPos.a)
        discard;
    
    float4 viewNormal = normalTarget.Sample(anisotropicSampler, UV);
    
    LightColor lightColor = (LightColor) 0.0f;
    CalculateLight3D(viewPos.xyz, viewNormal.xyz, 0, lightColor);
    
    float specCoef = specularTarget.Sample(anisotropicSampler, UV);
    
    outcolor.Diffuse = lightColor.diffuse + lightColor.ambient;
    outcolor.Specular = lightColor.specular * specCoef;
    
    outcolor.Diffuse.a = 1.0f;
    outcolor.Specular.a = 1.0f;
    
    return outcolor;
}
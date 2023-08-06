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
    
    float3 diffuse = (float3) 0.f;
    float3 specular = (float3) 0.f;
    
    float2 vUV = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 vViewPos = positionTarget.Sample(linearSampler, vUV);    
    if (0.f == vViewPos.a)
        discard;
      
    float4 albedo = albedoTarget.Sample(linearSampler, vUV);
    float4 normal = normalTarget.Sample(linearSampler, vUV);
    float metallic = saturate(metallicTarget.Sample(linearSampler, vUV).r);
    float roughness = saturate(roughnessTarget.Sample(linearSampler, vUV).r);

    LightColor lightcolor = (LightColor) 0.f;
    
    //if (0.f == metallic && 0.f == roughness)
    //{
    //    CalculateLight3D(vViewPos.xyz, normal.xyz, 0, lightcolor);
    //    output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
    //    output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
    //}
    //else
    {
        diffuse = CalculateLightPBR_Direct(vViewPos.xyz, albedo, normal.xyz, metallic, roughness);
//        specular = CalculateLightPBR_Specular(vViewPos.xyz, albedo, normal.xyz, metallic, roughness);
        output.vDiffuse.xyz = diffuse;
      //  output.vSpecular.xyz = specular;
    }
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
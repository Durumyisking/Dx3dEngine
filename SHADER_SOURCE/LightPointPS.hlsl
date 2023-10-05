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
    output.vSpecular = (float4) 0.f;
    
    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;

    
    float2 uv = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 worldPos = positionTarget.Sample(linearSampler, uv);
    if (0.f == worldPos.a)
        discard;
    
    float4 albedo = albedoTarget.Sample(linearSampler, uv);
    float3 normal = normalTarget.Sample(linearSampler, uv).xyz;
    float4 mrg = MRDTarget.Sample(linearSampler, uv);
    float metallic = saturate(mrg.r);
    float roughness = saturate(mrg.g);
    
    float pixelToCam = distance(cameraWorldPos.xyz, worldPos.xyz);
    float3 pixelToEye = normalize(cameraWorldPos.xyz - worldPos.xyz);
    ambientLighting = AmbientLightingByIBL(albedo.xyz, normal, pixelToEye, metallic, roughness, pixelToCam);
    
 
    float3 lightVec = lightAttributes[lightIndex].position.xyz - worldPos.xyz;
        
    float lightDist = length(lightVec);    
    
    if (lightDist > lightAttributes[lightIndex].fallOffEnd)
    {
        output.vDiffuse = (float4) 0.f;
        return output;
    }
    
    lightVec /= lightDist;
    
    // Distance attenuation
    float att = saturate((lightAttributes[lightIndex].fallOffEnd - lightDist)
                         / (lightAttributes[lightIndex].fallOffEnd - lightAttributes[lightIndex].fallOffStart));

        
    float3 radiance = lightAttributes[lightIndex].color.diffuse * att; // * shadowFactor;
    
    directLighting = PBR_DirectLighting(pixelToEye, lightVec, albedo.xyz, normal, metallic, roughness) * radiance;

    output.vDiffuse.xyz = ambientLighting + directLighting;// * lit;
    
    output.vDiffuse.a = 1.f;
    
    return output;
}
   
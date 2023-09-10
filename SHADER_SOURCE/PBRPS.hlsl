#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 WorldPos : POSITION; // Worldspace fragmentPos

    float3 WorldTangent : TANGENT; // World Space tangent
    float3 WorldNormal : NORMAL; // World Space normal
    float3 WorldBiNormal : BINORMAL; // World Space binormal

};

float4 main(VSOut vsIn) : SV_Target
{
    float3 outColor = (float3) 0.f;

    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;
    
    float4  albedo = float4(0.25f, 0.25f, 0.25f, 1.f);
    float3  normal = vsIn.WorldNormal;
    float   metallic = cbMetallic; 
    float   roughness = cbRoughness;
    float3  emission = (float3) 0.f;
    float3  A0 = (float3) 1.f;

    albedo = cbbAlbedo ? TextureMapping_albedo(vsIn.UV) : albedo;
    normal = cbbNormal ? TextureMapping_normal(vsIn.UV, vsIn.WorldTangent, vsIn.WorldNormal, vsIn.WorldBiNormal) : normal;
    metallic = cbbMetallic ? TextureMapping_metallic(vsIn.UV) : metallic;
    roughness = cbbRoughness ? TextureMapping_roughness(vsIn.UV) : roughness;
    emission = cbbEmissive ? TextureMapping_emissive(vsIn.UV) : emission;

    float3 pixelToEye = normalize(cameraWorldPos.xyz - vsIn.WorldPos);
    
    ambientLighting = AmbientLightingByIBL(albedo.xyz, normal, pixelToEye, metallic, roughness);
    
    float3 lightVec = -normalize(float4(lightAttributes[0].direction.xyz, 0.f)).xyz;

    float3 halfway = normalize(pixelToEye + lightVec);
        
    float NdotI = max(0.0, dot(normal, lightVec));
    float NdotH = max(0.0, dot(normal, halfway));
    float NdotO = max(0.0, dot(normal, pixelToEye));
        
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic);
    float3 F = fresnelSchlick(F0, max(0.0, dot(halfway, pixelToEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo.xyz;

    float D = ndfGGX(NdotH, roughness);
    float3 G = gaSchlickGGX(NdotI, NdotO, roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

    float3 radiance = lightAttributes[0].color.diffuse;

    directLighting += (diffuseBRDF + specularBRDF) * radiance * NdotI;
    

    //outColor.xyz = CalculateLightPBR_Direct(vsIn.WorldPos, albedo, normal, metallic, roughness);
    outColor.xyz = ambientLighting + directLighting + emission;

    
    return float4(outColor, 1.f);;

}

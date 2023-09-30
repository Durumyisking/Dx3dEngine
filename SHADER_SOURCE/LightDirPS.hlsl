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

/*
pos, normal, specular 타겟을 기반으로 빛 연산을 한다.
해당 쉐이더에서는 diffuse와 specular를 계산만 해준다.

*/

PS_OUT main(VSOut vsin)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;

    
    float2 uv = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 worldPos = positionTarget.Sample(linearSampler, uv);    
    if (0.f == worldPos.a)
        discard;
      
    float4 albedo = albedoTarget.Sample(linearSampler, uv);
    float4 normal = normalTarget.Sample(linearSampler, uv);
    float4 mrg = MRDTarget.Sample(linearSampler, uv);
    float metallic = saturate(mrg.r);
    float roughness = saturate(mrg.g);
    
    float pixelToCam = distance(cameraWorldPos.xyz, worldPos.xyz);
    float3 pixelToEye = normalize(cameraWorldPos.xyz - worldPos.xyz);
    ambientLighting = AmbientLightingByIBL(albedo.xyz, normal.xyz, pixelToEye, metallic, roughness, pixelToCam);
    
    
    // 현재 camera로 보고있는 projection 화면을
    // light 입장에서 본 proejction으로 바꿔야 한다.
    // ViewPos -> WorldPos
    
    // WorldPos -> Light 투영
    float4 lightProj = mul(float4(worldPos.xyz, 1.f), lightView);
    lightProj = mul(float4(lightProj.xyz, 1.f), lightProjection);

    lightProj.xy /=lightProj .w;    
    lightProj.z /= lightProj.w;

    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 depthMapUV = 
    float2(
        (lightProj.x * 0.5) + 0.5f,
        -(lightProj.y * 0.5) + 0.5f
    );
    float lit = 1.f;
    if (depthMapUV.x < 0.f || depthMapUV.x > 1.f || depthMapUV.y < 0.f || depthMapUV.y > 1.f)
    {
        lit = 1.f;
    }
    else
    {
        lit = VSM_FILTER(ShadowMap.Sample(linearSampler, depthMapUV).rg, lightProj.z);        
    }
        
    float3 lightVec = -normalize(float4(lightAttributes[0].direction.xyz, 0.f)).xyz;
    directLighting = PBR_DirectLighting(pixelToEye, lightVec, albedo.xyz, normal.xyz, metallic, roughness);

    output.vDiffuse.xyz = ambientLighting + directLighting * lit;
    
    output.vDiffuse.a = 1.f;
    //output.vSpecular.a = 1.f;
    
    return output;
}
   
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
    
    float3 diffuse = (float3) 0.f;
    float3 specular = (float3) 0.f;
    
    float2 vUV = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 vViewPos = positionTarget.Sample(linearSampler, vUV);    
    if (0.f == vViewPos.a)
        discard;
      
    float4 albedo = albedoTarget.Sample(linearSampler, vUV);
    float4 normal = normalTarget.Sample(linearSampler, vUV);
    float4 mrg = MRDTarget.Sample(linearSampler, vUV);
    float metallic = saturate(mrg.r);
    float roughness = saturate(mrg.g);
    

    LightColor lightcolor = (LightColor) 0.f;
        // 그림자 판정
    // ViewPos -> WorldPos
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), inverseView).xyz;

    // WorldPos -> Light 투영
    float4 vLightProj = mul(float4(vWorldPos, 1.f), lightView);
    vLightProj = mul(float4(vWorldPos, 1.f), lightProjection);

    // w 로 나눠서 실제 xy 투영좌표를 구함
    vLightProj.xy /= vLightProj.w;
    vLightProj.z /= vLightProj.w;

    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 vDepthMapUV = float2((vLightProj.x * 0.5) + 0.5f, -(vLightProj.y * 0.5) + 0.5f);
    float fDepth = ShadowMap.Sample(anisotropicSampler, vDepthMapUV).r;
    float fShadowPow = 0.f;

    //// 광원에 기록된 깊이보다, 물체의 깊이가 더 멀 때, 그림자 판정
    if (0.f != fDepth
        && 0.f <= vDepthMapUV.x && vDepthMapUV.x <= 1.f
        && 0.f <= vDepthMapUV.y && vDepthMapUV.y <= 1.f
        && vLightProj.z >= fDepth + 0.0001f)
    {
        fShadowPow = 0.9f;
    }
    
    
    {
        diffuse = CalculateLightPBR_Direct(vViewPos.xyz, albedo, normal.xyz, metallic, roughness);
        output.vDiffuse.xyz = diffuse;
    }
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
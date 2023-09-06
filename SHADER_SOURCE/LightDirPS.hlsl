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
    
    float2 uv = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 viewPos = positionTarget.Sample(linearSampler, uv);    
    if (0.f == viewPos.a)
        discard;
      
    //float4 albedo = albedoTarget.Sample(linearSampler, uv);
    float4 normal = normalTarget.Sample(linearSampler, uv);
    //float4 mrg = MRDTarget.Sample(linearSampler, uv);
    //float metallic = saturate(mrg.r);
    //float roughness = saturate(mrg.g);
    

    LightColor lightcolor = (LightColor) 0.f;
    lightcolor.diffuse = float4(0.25f, 0.25f, 0.25f, 1.f);
    lightcolor.specular = 0.f;
    lightcolor.ambient = float4(0.25f, 0.25f, 0.25f, 1.f);
    CalculateLight3D(viewPos.xyz, normal.xyz, 0, lightcolor);
    // 그림자 판정
    // ViewPos -> WorldPos
    float3 worldPos = mul(float4(viewPos.xyz, 1.f), inverseView).xyz;

    // WorldPos -> Light 투영
    float4 lightProj = mul(float4(worldPos, 1.f), lightView);
    lightProj = mul(float4(worldPos, 1.f), lightProjection);

    // w 로 나눠서 실제 xy 투영좌표를 구함
    lightProj .xy /=lightProj .w;
    lightProj.z /= lightProj.w;

    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 depthMapUV = float2((lightProj.x * 0.5) + 0.5f, -(lightProj.y * 0.5) + 0.5f);
    float depth = ShadowMap.Sample(linearSampler, depthMapUV).r;
    float shadowPow = 0.f;

    //// 광원에 기록된 깊이보다, 물체의 깊이가 더 멀 때, 그림자 판정
    if (0.f != depth
        && 0.f <= depthMapUV.x && depthMapUV.x <= 1.f
        && 0.f <= depthMapUV.y && depthMapUV.y <= 1.f
        && lightProj.z >= depth + 0.0001f)
    {
        shadowPow = 0.9f;
    }
    
    {
        //diffuse = CalculateLightPBR_Direct(viewPos.xyz, albedo, normal.xyz, metallic, roughness);
        //output.vDiffuse.xyz = diffuse * (1.f - shadowPow);
        
        output.vDiffuse = lightcolor.diffuse * (1.f - shadowPow) + lightcolor.ambient;
        output.vSpecular = lightcolor.specular;

    }
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
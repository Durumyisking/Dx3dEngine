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
    
    
    // 현재 camera로 보고있는 projection 화면을
    // light 입장에서 본 proejction으로 바꿔야 한다.
    // ViewPos -> WorldPos
    float3 worldPos = mul(float4(viewPos.xyz, 1.f), inverseView).xyz;

    // WorldPos -> Light 투영
    float4 lightProj = mul(float4(worldPos, 1.f), lightView);
    lightProj = mul(float4(lightProj.xyz, 1.f), lightProjection);

    lightProj.xy /=lightProj .w;
    
    // 광원으로부터 픽셀까지의 거리 ( 해당 거리보다 shadowmap에 저장된 값이 더 커야 그림자 생성)
    lightProj.z /= lightProj.w;

    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 depthMapUV = 
    float2(
    (lightProj.x * 0.5) + 0.5f,
    -(lightProj.y * 0.5) + 0.5f
    );
    
    float lit = VSM_FILTER(ShadowMap.Sample(shadowPointSampler, depthMapUV).rg, lightProj.z);
    
    float shadowPow = 0.f;
    
    {        
        output.vDiffuse = lightcolor.diffuse * lit + lightcolor.ambient;
        output.vSpecular = lightcolor.specular;
    }
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
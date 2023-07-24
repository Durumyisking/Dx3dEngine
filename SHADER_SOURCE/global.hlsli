#include "Texture.hlsli"
#include "Light.hlsli"
#include "Particle.hlsli"
#include "Sampler.hlsli"


struct VTX_IN
{
    float4 vPos : POSITION;                                                     
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;                                
};

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vWorldPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};



#define ZERO    float4(0.f, 0.f, 0.f, 0.f)

#define RED     float4(1.f, 0.f, 0.f, 1.f)
#define GREEN   float4(0.f, 1.f, 0.f, 1.f)
#define BLUE    float4(0.f, 0.f, 1.f, 1.f)
#define WHITE   float4(1.f, 1.f, 1.f, 1.f)
#define BLACK   float4(0.f, 0.f, 0.f, 1.f)
#define GREY    float4(0.5f, 0.5f, 0.5f, 1.f)
#define YELLOW  float4(1.f, 1.f, 0.f, 1.f)
#define MAGENTA float4(1.f, 0.f, 1.f, 1.f)
#define ALPHA(target, value) float4(target.r, target.g, target.b, (float)value)


float3 RotatePointZ(float3 pointVal, float radian, float3 rotationCenter)
{
    float cosTheta = cos(radian);
    float sinTheta = sin(radian);
    float3 rotatedPoint;
    
    float3 translatedPoint = pointVal - rotationCenter;

    rotatedPoint.x = translatedPoint.x * cosTheta - translatedPoint.y * sinTheta;
    rotatedPoint.y = translatedPoint.x * sinTheta + translatedPoint.y * cosTheta;
    rotatedPoint.z = translatedPoint.z;
    
    return rotatedPoint + rotationCenter;
}


float4 TextureMapping_albedo(float2 uv)
{
    return colorTexture.SampleLevel(anisotropicSampler, uv, 0.f);
}


// 현재 픽셀의 normal을 얻고 viewspace로 변경한다.
float3 TextureMapping_normal(float2 uv, float3 viewTangent, float3 viewNormal, float3 viewBiNormal)
{
    float3 result = normalTexture.SampleLevel(anisotropicSampler, uv, 0.f).xyz;
    
    result.xyz = normalize((result.xyz * 2.f).xyz - 1.f);
        

    float3x3 matTBN =
    {
        viewTangent,
        viewBiNormal,
        viewNormal,
    };

    result = normalize(float3(mul(result.xyz, matTBN)));
    
    return result;
}

float TextureMapping_metallic(float2 uv)
{
    return saturate(MetalTexture.SampleLevel(anisotropicSampler, uv, 0.f).r);
}

float TextureMapping_roughness(float2 uv)
{
    return saturate(RoughnessTexture.SampleLevel(anisotropicSampler, uv, 0.f).r);
}
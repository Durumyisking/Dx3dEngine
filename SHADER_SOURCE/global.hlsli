#include "Light.hlsli"
#include "Particle.hlsli"


struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vWorldPos : POSITION;
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

#define RESOLUTION float2(1600.f, 900.f)


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

float4 DecodeColor(float _value)
{
    uint rgba = asint(_value);

    float r = float((rgba & 0xff000000) >> 24) / 255.f;
    float g = float((rgba & 0x00ff0000) >> 16) / 255.f;
    float b = float((rgba & 0x0000ff00) >> 8) / 255.f;
    float a = float((rgba & 0x000000ff) >> 0) / 255.f;

    return float4(r, g, b, a);
}


float4 TextureMapping_albedo(float2 uv, float pixelToCam)
{
    float mip = pixelToCam / 10.f;
    if (mip > 10)
        mip = 10;

    return colorTexture.SampleLevel(linearSampler, uv, 0.f);
}


float3 TextureMapping_normal(float2 uv, float3 TangentWorld, float3 NormalWorld, float pixelToCam)
{
    float3 normal = normalTexture.SampleLevel(linearSampler, uv, 0.f).rgb;    
    normal = (normal * 2.f) - 1.f;
    
    // 마리오가 아마 vulkan으로 만든거라서 노말 y 뒤집어줘야할거임
    //normal.y = -normal.y;
    
    float3 N = NormalWorld;
    float3 T = normalize(TangentWorld - dot(TangentWorld, N) * N);
    float3 B = cross(N, T);
        
    float3x3 TBN = float3x3(T, B, N);
        
    return normalize(mul(normal, TBN));
}

float TextureMapping_metallic(float2 uv, float pixelToCam)
{
    return metallicTexture.Sample(linearSampler, uv).r;
}

float TextureMapping_roughness(float2 uv, float pixelToCam)
{
    return roughnessTexture.Sample(linearSampler, uv).r;
}

float3 TextureMapping_emissive(float2 uv, float pixelToCam)
{
    return emissiveTexture.Sample(linearSampler, uv).rgb;
}


// 3d위치나 방향을 구면 매핑 좌표로 변환합니다.
// 해당 좌표를 텍스처의 샘플링에 사용합니다.

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


//float4 TexcoordToView(float2 texcoord)
//{
//    float4 posProj;

//    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
//    posProj.xy = texcoord * 2.0 - 1.0;
//    posProj.y *= -1; // y 좌표 뒤집기
//    posProj.z = depthOnlyTex.Sample(clampSampler, texcoord).r;
//    posProj.w = 1.0;
    
//    // ProjectSpace -> ViewSpace
//    //float4 posView = mul(posProj, lights[0].invProj);
//    float4 posView = mul(posProj, inverseProjection);
//    posView.xyz /= posView.w;
    
//    return posView;
//}



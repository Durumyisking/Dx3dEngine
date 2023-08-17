#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

//SamplerState splr : register(s0);

static const float2 invAtan = float2(0.1591, 0.3183); 

// 3d위치나 방향을 구면 매핑 좌표로 변환합니다.
// 해당 좌표를 텍스처의 샘플링에 사용합니다.
float2 SampleSphericalMap(float3 v) // v는 픽셀 월드상의 위치
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y)); // 
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(VSOut psIn) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
    float3 color = CubeMapTexture.Sample(skyBoxSampler, normalize(psIn.WorldPos)).rgb;
		
    return float4(color, 1.0);
}
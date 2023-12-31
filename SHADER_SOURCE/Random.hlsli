#include "ConstantBuffer.hlsli"
#include "Blur.hlsli"

float Rand(float2 co)
{
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}


float4 GetRandomFromBlur(float2 UV)
{
    UV.x += elapsedTime; // 현재 uv.x에 경과시간 더함 랜덤값 받으려고 하는거
    UV.y += sin((UV.x + elapsedTime) * 3.14592f + 2.0f * 10.0f) * 0.5f; // 얘도
    
    float4 random = float4
                (
                    GaussianBlur(UV + float2(0.0f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.1f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.2f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.3f, 0.0f)).x
                ); // 대충 현재 uv값 부터 0.1 0.2까지의 x좌표를 넣어줌
    
    return random;
}


float4 GetRandomFloat4FromNoise(uint threadIDX, uint threadIDY)
{
    float seedx = ((float) threadIDX / (float) maxParticles) + elapsedTime;
    float seedy = ((float) threadIDY / (float) maxParticles) + elapsedTime;
    float r1 = Rand(float2(seedx, seedy));
    float r2 = Rand(float2(seedy, seedx));
    float r3 = Rand(float2(seedx * elapsedTime, seedy));
    float r4 = Rand(float2(seedx, seedy * elapsedTime));
    float r5 = Rand(float2(seedx * elapsedTime, seedy * elapsedTime));
            // [0.5~1] -> [0~1]
    float4 noise =
    {
        2.f * r1 - 1.f,
        2.f * r2 - 1.f,
        2.f * r3 - 1.f,
        2.f * r4 - 1.f
    };    
    
    return noise;
}

int GetRandomIntFromNoise(uint threadIDX, uint threadIDY, int start, int range)
{
    float seedx = ((float) threadIDX / (float) maxParticles) + elapsedTime;
    float seedy = ((float) threadIDY / (float) maxParticles) + elapsedTime;
    float r1 = Rand(float2(seedx, seedy));
        
   // [0~1] -> [0~Range]
    float noise = (r1 - 0.5f) * 2.f;
    noise *= range;
    
    // [0~Range] -> [Start -> Start+Range]
    int result = (int) noise;
    result += start;
    
    return result;
}
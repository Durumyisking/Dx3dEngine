#include "Quaternion.hlsl"

struct Particle
{
    float4 position;
    float4 direction;
    float4 startColor;
    float4 endColor;

    float3 startScale;
    float3 endScale;
    
    float lifeTime;
    float elapsedTime;
    
    float4 q_startRotation;
    float4 q_endRotation;
    
    float speed;
    float radian;
    float gravityAcc;
    uint active;
    
    matrix particleWorld;
};


struct ParticleShared
{
    uint gActiveCount;
};

StructuredBuffer<Particle> particleBuffer : register(t16);
RWStructuredBuffer<Particle> ParticleBufferUAV : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBufferUAV : register(u1);

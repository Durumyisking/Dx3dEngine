#include "Particle.hlsli"
#include "Random.hlsli"


void ParticleThreadSync(uint threadID)
{
    while (0 < ParticleSharedBufferUAV[0].gActiveCount)
    {
        int originValue = 0;
        int currentSharedBufferActiveCount = ParticleSharedBufferUAV[0].gActiveCount;
        if (currentSharedBufferActiveCount <= 0)
        {
            break;
        }
        int expected = currentSharedBufferActiveCount;
        int exchange = currentSharedBufferActiveCount - 1;
                    
        InterlockedCompareExchange(ParticleSharedBufferUAV[0].gActiveCount
                                        , expected, exchange, originValue);
            
        if (originValue == expected)
        {
            ParticleBufferUAV[threadID].active = 1;
            break;
        }
    }
}

void InitalizeParticleBufferUAV(uint threadID, float3 position, float4 direction, float3 scale, float4 startColor, float4 endColor, float lifeTime, float speed, float radian)
{
    ParticleBufferUAV[threadID].position.xyz = position;
    
    if (simulationSpace) // 1 world , 0 local
    {
        ParticleBufferUAV[threadID].position.xyz += worldPosition.xyz;
    }

    direction = normalize(direction);
    ParticleBufferUAV[threadID].direction = direction;
    ParticleBufferUAV[threadID].startScale = scale;
    ParticleBufferUAV[threadID].startColor= startColor;
    ParticleBufferUAV[threadID].endColor = endColor;
//    ParticleBufferUAV[threadID].lifeTime = lifeTime;
    ParticleBufferUAV[threadID].gravityAcc = 0.f;
    ParticleBufferUAV[threadID].elapsedTime = 0.f;
    ParticleBufferUAV[threadID].speed = speed;
    ParticleBufferUAV[threadID].radian = radian;
}
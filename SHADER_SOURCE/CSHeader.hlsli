#include "Particle.hlsli"
#include "Random.hlsli"


void ParticleThreadSync(uint threadID)
{
    while (0 < ParticleSharedBufferUAV[0].gActiveCount)
    {
        int originValue = 0;
        int CurrentSharedBufferActiveCount = ParticleSharedBufferUAV[0].gActiveCount;
        if (CurrentSharedBufferActiveCount <= 0)
        {
            break;
        }
        int expected = CurrentSharedBufferActiveCount;
        int exchange = CurrentSharedBufferActiveCount - 1;
                    
        InterlockedCompareExchange(ParticleSharedBufferUAV[0].gActiveCount
                                        , expected, exchange, originValue);
            
        if (originValue == expected)
        {
            ParticleBufferUAV[threadID].active = 1;
            break;
        }
    }
}

void InitalizeParticleBufferUAV(uint threadID, float3 _Position, float4 _Direction, float3 _Scale, float4 _StartColor, float4 _EndColor, float _LifeTime, float _Speed, float _Radian)
{
    ParticleBufferUAV[threadID].position.xyz = _Position;
    
    if (simulationSpace) // 1 world , 0 local
    {
        ParticleBufferUAV[threadID].position.xyz += worldPosition.xyz;
    }

    _Direction = normalize(_Direction);
    ParticleBufferUAV[threadID].direction = _Direction;
    ParticleBufferUAV[threadID].startScale = _Scale;
    ParticleBufferUAV[threadID].startColor= _StartColor;
    ParticleBufferUAV[threadID].endColor = _EndColor;
//    ParticleBufferUAV[threadID].lifeTime = _LifeTime;
    ParticleBufferUAV[threadID].gravityAcc = 0.f;
    ParticleBufferUAV[threadID].elapsedTime = 0.f;
    ParticleBufferUAV[threadID].speed = _Speed;
    ParticleBufferUAV[threadID].radian = _Radian;
}
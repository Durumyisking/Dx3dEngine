#include "CSHeader.hlsli"


[numthreads(128, 1, 1)] // 그룹당 쓰레드 개수 ( 우리는 그룹 1개쓰니까 128개만 쓰는거 )
void main(uint3 DTid : SV_DispatchThreadID) // 쓰레드 그룹 xyz를 인자로 받음
{
    if (maxParticles <= DTid.x) // 쓰레드 넘버 x보다 작으면 return (우린 x만쓰니까) 부등호 바뀌어야하나???
        return;
    
    if (ParticleBufferUAV[DTid.x].active == 0) // 파티클버퍼 index x가 활성화가 아니면
    {
        ParticleThreadSync(DTid.x);
        
        if (ParticleBufferUAV[DTid.x].active) // 위에서 성공하면 여기로 들어옴
        {
            float seedx = DTid.x;
            float seedy = DTid.y;
            float r1 = Rand(float2(seedx, seedy));
            float r2 = Rand(float2(seedy, seedx));
            float r3 = Rand(float2(seedx * elapsedTime, seedy));
            float r4 = Rand(float2(seedx, seedy * elapsedTime));
            float r5 = Rand(float2(seedx * elapsedTime, seedy * elapsedTime));
            // [0.5~1] -> [0~1]
            
            float2 random = float2(r1, r2);
            float2 Theta = random * 3.141592f * 2.0f;
            
            
            ParticleBufferUAV[DTid.x].position.xz = float2(cos(Theta.x), sin(Theta.y)) * random.y * radius;
            ParticleBufferUAV[DTid.x].position.y = 1.0f; // y값은 고정
            
            if (simulationSpace) // 1 world , 0 local
            {
                ParticleBufferUAV[DTid.x].position.xyz += world._41_42_42;
            }
            
            ParticleBufferUAV[DTid.x].q_startRotation = startAngle;
            ParticleBufferUAV[DTid.x].q_endRotation = endAngle;
            
            ParticleBufferUAV[DTid.x].startScale = startSize;
            ParticleBufferUAV[DTid.x].endScale = endSize;
            
            //파티클 속력
            ParticleBufferUAV[DTid.x].elapsedTime = 0.f;
            
            float4 noise =
            {
                2.f * r1 - 1.f,
                2.f * r2 - 1.f,
                2.f * r3 - 1.f,
                2.f * r4 - 1.f
            };
            
          ParticleBufferUAV[DTid.x].speed = startSpeed;
            
          ParticleBufferUAV[DTid.x].direction = worldPosition +ParticleBufferUAV[DTid.x].position;
          ParticleBufferUAV[DTid.x].direction.xyz += world._41_42_43;
          ParticleBufferUAV[DTid.x].direction.w = 1.0f;
          ParticleBufferUAV[DTid.x].direction = normalize(ParticleBufferUAV[DTid.x].direction);
            
          ParticleBufferUAV[DTid.x].lifeTime = (maxLifeTime - minLifeTime) * (2.f * r5 - 1.f) + minLifeTime;
          if (minLifeTime == 0.f)
              ParticleBufferUAV[DTid.x].lifeTime = maxLifeTime;
        }
    }
    else // active == 1
    {
        ParticleBufferUAV[DTid.x].elapsedTime += deltaTime;
        if (ParticleBufferUAV[DTid.x].lifeTime < ParticleBufferUAV[DTid.x].elapsedTime)
        {
            ParticleBufferUAV[DTid.x].active = 0;
        }
        else
        {
            //ParticleBufferUAV[DTid.x].position
            //+= ParticleBufferUAV[DTid.x].direction * ParticleBufferUAV[DTid.x].speed * deltaTime;
            
            float3 scale = lerp(ParticleBufferUAV[DTid.x].startScale, ParticleBufferUAV[DTid.x].endScale, ParticleBufferUAV[DTid.x].elapsedTime / ParticleBufferUAV[DTid.x].lifeTime);
            
             // scale
            row_major matrix worldMatrix = matrix
            (scale.x, 0.0f, 0.0f, 0.0f
            , 0.0f, scale.y, 0.0f, 0.0f
            , 0.0f, 0.0f, scale.z, 0.0f
            , 0.0f, 0.0f, 0.0f, 1.0f);
            
            // rotation
            float4 q_startAngle = EulerToQuternion(ParticleBufferUAV[DTid.x].q_startRotation.xyz);
            float4 q_endAngle = EulerToQuternion(ParticleBufferUAV[DTid.x].q_endRotation.xyz);
            
            float4 q_rotation = q_slerp(q_startAngle, q_endAngle, ParticleBufferUAV[DTid.x].elapsedTime / ParticleBufferUAV[DTid.x].lifeTime);
            float4x4 rotationMatrix = quaternion_to_matrix(q_rotation);
            
            //  S * R
            ParticleBufferUAV[DTid.x].particleWorld = mul(worldMatrix, rotationMatrix);
            
            
            // translation
            //worldMatrix._41_42_43 = ParticleBufferUAV[DTid.x].position.xyz;
            //ParticleBufferUAV[DTid.x].particleWorld = worldMatrix;
        }
    }
}

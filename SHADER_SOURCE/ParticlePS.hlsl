#include "global.hlsli"

struct GSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};


float4 main(GSOutput vsIn) : SV_TARGET
{      
    float4 outColor = (float4) 0.0f;
    
    float fElapsedTime = particleBuffer[vsIn.Instance].elapsedTime;
    float fLifeTime= particleBuffer[vsIn.Instance].lifeTime;

    
    float alpharatio = 1.f - fElapsedTime / fLifeTime;

    outColor = particleBuffer[vsIn.Instance].startColor;

    outColor.a = alpharatio;
    
//    outColor = lerp(startColor, endColor, particleBuffer[In.Instance].lifeTime);

//    outColor.w = alpharatio;
    
    if (outColor.w == 0.f)
        discard;

    
    return outColor;
}
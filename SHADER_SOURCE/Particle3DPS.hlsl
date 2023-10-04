#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    
    uint Instance : SV_InstanceID; // drawindexedinstance 하면 자동으로 들어간다.
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    float3 ViewPos : POSITION;

    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    
    uint Instance : SV_InstanceID;
};


float4 main(VSOut vsIn) : SV_TARGET
{
    float4 outColor = (float4) 0.0f;
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    float fElapsedTime = particleBuffer[vsIn.Instance].elapsedTime;
    float fLifeTime = particleBuffer[vsIn.Instance].lifeTime;

    
    float alpharatio = 1.f - fElapsedTime / fLifeTime;

    // outColor = particleBuffer[vsIn.Instance].startColor;
    outColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
    outColor.a = alpharatio;
    
//    outColor = lerp(startColor, endColor, particleBuffer[In.Instance].lifeTime);

//    outColor.w = alpharatio;
    
    if (outColor.w <= 0.1f)
        discard;

    
    return outColor;
}
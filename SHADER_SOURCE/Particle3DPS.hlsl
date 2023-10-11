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
    
    float fElapsedTime = particleBuffer[vsIn.Instance].elapsedTime;
    float fLifeTime = particleBuffer[vsIn.Instance].lifeTime;

    if (particleBuffer[vsIn.Instance].active == 0)
        discard;
    
    //float alpharatio = 1.f - fElapsedTime / fLifeTime;

    if (1 == cbbAlbedo)
    {
        float2 startUV = float2(float(particleBuffer[vsIn.Instance].texture_x_index) / cbxy1.x, float(particleBuffer[vsIn.Instance].texture_y_index) / cbxy1.y);
        float2 endUV = float2(float(particleBuffer[vsIn.Instance].texture_x_index + 1) / cbxy1.x, float(particleBuffer[vsIn.Instance].texture_y_index + 1) / cbxy1.y);
    
        float UV = abs(endUV - startUV) * vsIn.UV;
        UV += startUV;
    
        outColor = colorTexture.Sample(linearSampler, UV);
    }
    
    //outColor.w = alpharatio;
    if (outColor.w <= 0.1f)
        discard;

    return outColor;
}
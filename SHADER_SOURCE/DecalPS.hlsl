#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITIONT;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 Projection : POSITION0;
    float4 worldView : POSITION1;
};


float4 main(VSOut _in) : SV_TARGET
{
    float4 output = (float4) 0.f;
    
    float2 screenPos = _in.Projection.xy / _in.Projection.w;
    
    float2 UV = screenPos * float2(0.5f, -0.5f) + 0.5f;
    float depth = DepthMap.Sample(shadowPointSampler, UV);
    
    float3 viewRay = _in.worldView.xyz * (1000.f / -_in.worldView.z);
    float3 viewPos = viewRay * depth;
    
    float4 worldPos = mul(float4(viewPos, 1.0f), inverseView);
    float4 localPos = mul(worldPos, inverseWorld);
    
    
    
    
    
    // decal 박스의 밖 이면 클리핑
    // 0 보다 작으면 픽셀 삭제
    clip(0.5f - abs(localPos.xyz));
    
    // 데칼 박스 범위가 -0.5 ~ 0.5 사이이므로 0 ~ 1사이의 UV 좌표로만들어줌
    //float2 decalUV = invLocalPos.xz + 0.5f;
    return float4(localPos.x, localPos.y, localPos.z, 1.0f);
    
    return output;
}
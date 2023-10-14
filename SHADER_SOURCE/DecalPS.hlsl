#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITIONT;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 Projection : POSITION0;
};


float4 main(VSOut _in) : SV_TARGET
{
    float4 output = (float4) 0.f;
    
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    // decal box의 위치를 UV 좌표로 변환
    float3 screenposition = _in.Projection.xyz / _in.Projection.w;
    screenposition.x = screenposition.x * 0.5 + 0.5;
    screenposition.y = -screenposition.y * 0.5 + 0.5;
    
    // Depth탐색
    float depth = DepthMap.Sample(shadowPointSampler, screenposition.xy);
    float viewZ = depth * 1000.f;
    
    // 깊이를 로컬좌표로 변환
    float4 invProjecPos = (float4)0.0f;
    
    invProjecPos.x = (screenposition.x * 2.f - 1.f) * viewZ;
    invProjecPos.y = (screenposition.y * -2.f + 1.f) * viewZ;
    invProjecPos.z = depth * viewZ;
    invProjecPos.w = viewZ;
    
    float4 invVeiwPos = mul(invProjecPos, inverseProjection);
    float4 invWorldPos = mul(invVeiwPos, inverseView);
    float4 invLocalPos = mul(invWorldPos, inverseWorld);
    
    // decal 밖의 밖 이면 클리핑
    float3 ObjPos = abs(invLocalPos.xyz);
    
    // 0 보다 작으면 픽셀 삭제
    clip(0.5f - ObjPos);
    
    // 데칼 박스 범위가 -0.5 ~ 0.5 사이이므로 0 ~ 1사이의 UV 좌표로만들어줌
    float2 decalUV = invLocalPos.xz + 0.5f;
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return output;
}
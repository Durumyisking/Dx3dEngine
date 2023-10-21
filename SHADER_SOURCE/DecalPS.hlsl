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
    
    float2 screenPos = _in.Position.xy / _in.Position.w;
    
    float2 UV = screenPos * float2(0.5f, -0.5f) + 0.5f;
    //float depth = DepthMap.Sample(shadowPointSampler, UV);
    
    //float3 viewRay = _in.worldView.xyz * (1000.f / -_in.worldView.z);
    //float3 viewPos = viewRay * depth;
    
    //float4 worldPos = mul(float4(viewPos, 1.0f), inverseView);
    //float4 localPos = mul(worldPos, inverseWorld);

    //return float4(clippos.x, clippos.y, clippos.z, 1.0f);
    float4 col = float4(1.0, 0.0, 0.0, 0.0);

    float3 screenposition = _in.Projection.xyz / _in.Projection.w;
    screenposition.x = screenposition.x * 0.5 + 0.5;
    screenposition.y = -screenposition.y * 0.5 + 0.5;
    
    float depth = DepthMap.Sample(shadowPointSampler, UV);

    float4 cameraRay = float4(1.0f, -1.0f, 1.0f, 1.0f);

    float3 ViewRay = float3(lerp(-cameraRay.xy, cameraRay.xy, screenposition.xy).xy, cameraRay.z);

    float4 ViewPos = float4(ViewRay.xyz * depth, 1.0);
    float4 ObjPos = mul(ViewPos, inverseView);
    
    return float4(ObjPos.x, ObjPos.y, ObjPos.z, 1.0f);
    
    ObjPos = mul(ObjPos, inverseWorld);
    
    //float4 test = mul(_in.Projection, inverseProjection);
    //test = mul(test, inverseView);
    

    float3 ObjAbs = abs(ObjPos.xyz);
    clip(0.5 - ObjAbs);

    return float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    // decal 박스의 밖 이면 클리핑
    // 0 보다 작으면 픽셀 삭제
    
    // 데칼 박스 범위가 -0.5 ~ 0.5 사이이므로 0 ~ 1사이의 UV 좌표로만들어줌
    //float2 decalUV = invLocalPos.xz + 0.5f;
    
    return output;
}
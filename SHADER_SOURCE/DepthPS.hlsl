#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
    float4 WVP : POSITION1;
};

struct PSOut
{
    float4 depthColor : SV_Target0;
    float4 depthMap : SV_Target1;
};

PSOut main(VSOut vsIn) : SV_TARGET
{
    PSOut psOut = (PSOut) 0.f;
    
    float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 픽셀쉐이더로 넘어오기 전에는 깊이(z)는 원근나누기 처리 해주지 않고 넘어온다.
    // 따라서 지금 해줘야함
    float depth = vsIn.ProjPosition.z / vsIn.ProjPosition.w; // z값이 클수록 더 큰 값이 나온다
    //output = (depth.xxx, 1.0f);
    psOut.depthColor.r = depth;
    // decal
    psOut.depthMap.r = vsIn.WVP.z / 1000.f;
    
    // vsm 쓸때 풀기
    //psOut.depthColor.g = depth * depth;
    //output.rgb = In.ProjPosition.z / In.ProjPosition.w;
    //output.rgb *= (900 / 1600);
    
    return psOut;
}
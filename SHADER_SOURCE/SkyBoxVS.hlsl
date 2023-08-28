#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_POSITION;
    float3 WorldPos : Position;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut;

    matrix v = view;
    v[3][0] = 0;
    v[3][1] = 0;
    v[3][2] = 0;
    // 이동 안한다는 뜻인듯?
    
    vsOut.WorldPos = vsIn.Position.xyz;
    vsOut.Position = mul(float4(vsIn.Position.xyz, 1.0), mul(v, projection));
    
    // 원근 나누기
    vsOut.Position.z = vsOut.Position.w;
    return vsOut;

}

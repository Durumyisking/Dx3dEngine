#include "global.hlsli"

struct VSIn
{
    float4 Position : Position;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};


VSOut main(VSIn vsIn)
{
    VSOut vsOut;
    
    matrix v = view;
    v[3][0] = 0;
    v[3][1] = 0;
    v[3][2] = 0;

    
    vsOut.WorldPos = vsIn.Position;
    vsOut.Position = mul(float4(vsIn.Position.xyz, 1.0), v);
    vsOut.Position.z = vsOut.Position.w;

    return vsOut;
}


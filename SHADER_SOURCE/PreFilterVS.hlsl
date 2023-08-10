#include "global.hlsli"

struct VSIn
{
    float4 Position : Position;
};

struct VSOut
{
    float4 ProjPosition : SV_Position;
    float4 WorldPosition : Position;
};


cbuffer Cbuff : register(b0)
{
    matrix m;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut;
    
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);

    vsOut.WorldPosition = worldPosition;
    vsOut.ProjPosition = projPosition;
//    vsOut.ProjPosition.z = projPosition.w;

    return vsOut;
}


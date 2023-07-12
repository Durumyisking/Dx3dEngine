#include "global.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut vsIn) : SV_TARGET
{
    float4 vsOut = float4(1.f, 1.f, 1.f, 1.f);
        
    float thickness = cbfData1;
    float2 gridOffset = cbxy1;
    float2 resolution = cbxy2;
    
    const int width = (gridOffset.x * (resolution.y / resolution.x));
    const int height = (gridOffset.y * (resolution.y / resolution.x));

    if ((abs(vsIn.WorldPos.x % width) <= thickness) ||
        (abs(vsIn.WorldPos.y % height) <= thickness))
    {
        if (-2.f <= vsIn.WorldPos.y && 2.f >= vsIn.WorldPos.y)
        {
            vsOut = float4(0.f, 1.f, 0.f, 1.f);
        }
        else if (-2.f <= vsIn.WorldPos.x && 2.f >= vsIn.WorldPos.x)
        {
            vsOut = float4(1.f, 0.f, 0.f, 1.f);
        }
        
        return vsOut;
    }
    else
        discard;
    
    vsOut = float4(1.f, 0.f, 0.f, 1.f);
    
    return vsOut;
}
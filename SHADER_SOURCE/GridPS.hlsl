#include "global.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 Out = float4(1.f, 1.f, 1.f, 1.f);
        
    float thickness = cbfData1;
    float2 grid_offset = cbxy1;
    float2 resolution = cbxy2;
    
    const int width = (grid_offset.x * (resolution.y / resolution.x));
    const int height = (grid_offset.y * (resolution.y / resolution.x));

    if ((abs(In.WorldPos.x % width) <= thickness) ||
        (abs(In.WorldPos.y % height) <= thickness))
    {
        if (-2.f <= In.WorldPos.y && 2.f >= In.WorldPos.y)
        {
            Out = float4(0.f, 1.f, 0.f, 1.f);            
        }
        else if (-2.f <= In.WorldPos.x && 2.f >= In.WorldPos.x)
        {
            Out = float4(1.f, 0.f, 0.f, 1.f);
        }
        
        return Out;        
    }
    else
        discard;
    
    Out = float4(1.f, 0.f, 0.f, 1.f);
    
    return Out;
}
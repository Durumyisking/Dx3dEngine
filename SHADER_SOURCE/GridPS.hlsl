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
    
    //return float4(1.f, 0.f, 1.f, 1.f);
    
    //float thickness = cbfData1;
    //float2 grid_offset = cbxy1;
    //float2 resolution = cbxy2;
    
    //const int width = grid_offset.x;
    //const int height = (grid_offset.y * (resolution.y / resolution.x));

    //if ((abs(In.WorldPos.x % width) <= thickness) ||
    //    (abs(In.WorldPos.y % height) <= thickness))
    //    return Out;
    //else
    //    discard;
    
    //Out.y = 0.f;
    
    return float4(1.f, 0.f, 1.f, 1.f);
}
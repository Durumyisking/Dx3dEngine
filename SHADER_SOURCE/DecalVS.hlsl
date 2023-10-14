#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 Projection : POSITION0;
};


VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.f;
    
    float4 worldpos = mul(_in.Pos, world);
    float4 viewpos = mul(worldpos, view);
    float4 projectionpos = mul(viewpos, projection);
    
    output.Position = projectionpos;
    output.Projection= projectionpos;
    
    return output;
}
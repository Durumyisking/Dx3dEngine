#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 Projection : POSITION0;
    float4 worldView : POSITION1;
};


VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.f;
    
    float4 worldpos = mul(float4(_in.Pos.xyz, 1.0f), world);
    float4 viewpos = mul(worldpos, view);
    float4 projectionpos = mul(viewpos, projection);
    
    output.Position = projectionpos;
    output.Projection= projectionpos;
    output.worldView = viewpos;
    
    return output;
}
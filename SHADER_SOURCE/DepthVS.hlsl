#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
};



VSOut main(VSIn vsIn)
{
    VSOut output = (VSOut) 0.0f;
    //In.Position.w = 1.0f;
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, lightView);
    float4 ProjPosition = mul(viewPosition, lightProjection);
    
    output.Position = ProjPosition;
    output.ProjPosition = ProjPosition;
    
    return output;
}
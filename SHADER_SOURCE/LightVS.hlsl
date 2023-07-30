#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
};


VSOut main(VSIn vsIn)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Position = float4(vsIn.Position.xy * 2.0f, 0.0f, 1.0f);
    
    return Out;
}
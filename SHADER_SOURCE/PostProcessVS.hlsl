#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn vsIn)
{
    VSOut output = (VSOut) 0.f;
    
    output.Pos = float4(vsIn.Pos.xyz * 2.f, 1.0);
    output.UV = vsIn.UV;
    
    return output;
}
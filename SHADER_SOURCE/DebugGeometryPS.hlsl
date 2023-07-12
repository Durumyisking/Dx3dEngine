#include "global.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float2 Uv : TEXCOORD;
    float3 ViewPos : POSITION;
};

struct PSOut
{
    float4 Pos : SV_Target0;
    float4 Color : SV_Target1;
};


PSOut main(VSOut vsIn)
{
    PSOut output = (PSOut) 0.f;
    
    output.Pos = float4(vsIn.ViewPos, 0.f);
    output.Color = float4(1.f, 0.f, 0.f, 1.f);
    
    return output;
}
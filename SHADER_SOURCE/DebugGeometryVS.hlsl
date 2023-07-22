#include "global.hlsli"


struct VSIn
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 Uv : TEXCOORD;
    float3 ViewPos : POSITION;
};



VSOut main(VSIn vsIn) 
{
    VSOut output = (VSOut) 0.f;
    
    float4 worldPosition = mul(vsIn.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    output.Pos = projPosition;
    output.Uv = vsIn.Uv;
    output.ViewPos = viewPosition.xyz;
    
    return output;
}

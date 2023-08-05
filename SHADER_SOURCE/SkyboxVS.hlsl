#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : POSITION;   
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.f;
    
    vsOut.WorldPos = mul(vsIn.Position, world);
    float4 viewPosition = mul(vsOut.WorldPos, view);
    vsOut.Position = mul(viewPosition, projection);
        
    return vsOut;
}

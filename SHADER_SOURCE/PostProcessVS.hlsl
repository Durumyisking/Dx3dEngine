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
    
    //output.Pos = mul(float4(_in.Pos.xyz, 1.0f), world);
    
    float4 pos = mul(float4(vsIn.Pos), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    output.Pos = projPos;
    output.UV = vsIn.UV;
    
    return output;
}
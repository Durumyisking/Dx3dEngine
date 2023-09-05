#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 CubeUV : TEXCOORD;
};


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.CubeUV = In.Position.xyz;
    Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);

    
    return Out;
}
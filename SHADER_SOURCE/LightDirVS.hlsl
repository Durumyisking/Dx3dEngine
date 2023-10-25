#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    //float4 PlayerPosition : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position; // uv구하기 위한 ..
    //float4 PlayerDepth : SV_Position1;
};


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
    
    
    //Out.PlayerDepth = 
    
    return Out;
}
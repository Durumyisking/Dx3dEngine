#include "global.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{
    float4 output = (float4) 0.0f;

    //sphere
    //In.UV.y -= 0.575f;
    output = SkySphere.Sample(linearSampler, In.UV);
        
    return output;
}
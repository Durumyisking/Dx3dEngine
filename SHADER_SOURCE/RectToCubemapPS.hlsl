#include "global.hlsli"


struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};


float4 main(VSOut psIn) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
    float3 color = Skybox.Sample(skyBoxSampler, uv).rgb;
//  float3 color = float3(1.f, 0.f, 0.f);
		
    return float4(color, 1.0);
}
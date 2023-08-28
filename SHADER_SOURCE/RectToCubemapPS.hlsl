#include "global.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    float3 CUBEUV : TEXCOORD1;
};


float4 main(VSOut In) : SV_Target
{
    float4 output = (float4) 0.0f;


    //sphere
    output = Skybox.Sample(anisotropicSampler, In.UV);
    
    //Cube
    //output = Skybox.Sample(anisotropicSampler, In.CUBEUV);
    
    return output;
}




//struct VSOut
//{
//    float4 Position : SV_Position;
//    float3 WorldPos : Position;
//};


//float4 main(VSOut psIn) : SV_TARGET
//{
//    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
//    float3 color = Skybox.Sample(skyBoxSampler, uv).rgb;
////  float3 color = float3(1.f, 0.f, 0.f);
		
//    return float4(color, 1.0);
//}
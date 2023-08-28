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
    //output = CubeMapTexture.Sample(anisotropicSampler, In.CUBEUV);
    
    return output;
}


//struct VSOut
//{
//    float4 Position : SV_Position;
//    float3 WorldPos : Position;
//};

//SamplerState splr : register(s0);

//float4 main(VSOut psIn) : SV_TARGET
//{
//    float2 uv = SampleSphericalMap(normalize(psIn.WorldPos));
//    float3 color = CubeMapTexture.Sample(skyBoxSampler, normalize(psIn.WorldPos)).rgb;
		
//    return float4(color, 1.0);
//}
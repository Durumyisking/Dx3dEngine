#include "global.hlsli"
struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};
#define INTENSITY 0.4f


float3 FilmicToneMapping(float3 color)
{
    color = max(float3(0, 0, 0), color);
    color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
    return color;
}

float3 LinearToneMapping(float3 color)
{
                                        //gam
    float3 invGamma = float3(1, 1, 1) / 2.f;
                 //exp                  
    color = clamp(1.f * color, 0., 1.);
    color = pow(color, invGamma);
    return color;
}

float3 Uncharted2ToneMapping(float3 color)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
            //exp
    color *= 1.f;
    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
    float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    color /= white;                             //gam
    color = pow(color, float3(1.0, 1.0, 1.0) / 2.f);
    return color;
}

float4 main(VSOut vsIn) : SV_Target
{      
    float3 color = (float3) 0.f;
        
    // VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    float2 UV = vsIn.Pos.xy / RESOLUTION;
            
    color = postProcessTexture.Sample(linearSampler, vsIn.UV).xyz;

    color = LinearToneMapping(color);
    
    return float4(color, 1.f);
}
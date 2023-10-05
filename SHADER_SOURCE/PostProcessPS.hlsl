#include "PostProcess.hlsli"
#include "Blur.hlsli"

#define INTENSITY 0.4f
float4 main(VSOut vsIn) : SV_Target
{      
    float4 color = (float4) 0.f;
        
    // VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    float2 UV = vsIn.Pos.xy / Resolution;
            
    color = postProcessTexture.Sample(linearSampler, UV);
    
    //Pixels towards the edge get darker
    float dist = 1.0f - distance(float2((vsIn.Pos.x - UV.x / 2) / UV.x, (vsIn.Pos.y - UV.y / 2) / UV.y) * INTENSITY, float2(0.0f, 0.0f));
    color.xyz *= dist;
    
    /* Gamma correction */
    color.xyz = color.xyz / (color.xyz + float3(1.0, 1.0, 1.0));
    color.xyz = pow(color.xyz, float3(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0));

    //color.xyz = float3(1.f, 0.f, 0.f);
    
    return color;
}
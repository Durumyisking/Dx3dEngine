#include "PostProcess.hlsli"
#include "Blur.hlsli"

float4 main(VSOut vsIn) : SV_Target
{      
    float4 color = (float4) 0.f;
        
    // VS_OUT ���� ������ SV_Position ���� PixelShader �� �Էµ� �� �ȼ���ǥ�� ��ȯ�ؼ� �Է�
    float2 UV = vsIn.Pos.xy / Resolution;
        
    float2 fAdd = float2(NoiseTexture.Sample(pointSampler, vsIn.UV + ElapsedTime * 0.2f).x
                         , NoiseTexture.Sample(pointSampler, vsIn.UV + float2(0.1f, 0.f) + ElapsedTime * 0.2f).x);
    fAdd -= fAdd / 2.f;
    UV += fAdd * 0.05f;
    
    color = postProcessTexture.Sample(pointSampler, UV);
    
    return color;
}
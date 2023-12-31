#include "global.hlsli"

#define INVERSE -1

float4 main(VTX_OUT vsIn) : SV_Target
{   
    float4 color = (float) 0.f;
    
    if (cbiData1 == INVERSE)
        vsIn.vUV.x = 1.f - vsIn.vUV.x;

    
    if (0 == animationType)
    {
        color = colorTexture.SampleLevel(linearSampler, vsIn.vUV, 0.f);
    }
    else if (1 == animationType)
    {
        float2 diff = (atlasSize - spriteSize) / 2.f;
        float2 UV = (LT - diff - offset) + (atlasSize * vsIn.vUV);

        if (UV.x < LT.x || UV.y < LT.y || UV.x > LT.x + spriteSize.x || UV.y > LT.y + spriteSize.y)
            discard;
        
        color = atlasTexture.Sample(linearSampler, UV);
    }
    else if (2 == animationType)
    {
        
    }
    
    // Masking effect
    //if (0.f < NoiseTime)
    //{
    //    color.a -= NoiseTexture.Sample(anisotropicSampler, _in.vUV).x * 0.25f * (10.0f - NoiseTime);
    //}
    
    if(color.w == 0)
        discard;
    
    color *= cbxyzw1; // ���� ��
    color += cbxyzw2; // ���� ��
     
    LightColor lightcolor = (LightColor) 0.f;
    for (uint i = 0; i < lightCount; i++)
    {
        CalculateLight(lightcolor, vsIn.vWorldPos.xyz, i);
    }
    

    color *= lightcolor.diffuse;
    
     
    return color;
    
}
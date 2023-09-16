#include "global.hlsli"


struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float2 UV : TEXCOORD;
};

//cbuffer time
//int 0

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    if (animationType == 1)
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (LT - diff - offset) + (atlasSize * In.UV);
        
        if (UV.x < LT.x || UV.y < LT.y 
            || UV.x > LT.x + spriteSize.x 
            || UV.y > LT.y + spriteSize.y)
            discard;
        
        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else
    {
        //UV.x = -UV.x;
        color = colorTexture.Sample(anisotropicSampler, In.UV);
    }
    
    
    // noise, paper burn effect
    //if ( 0.0f < NoiseTime )
    //{
    //    color.a -= NoiseTexture.Sample(anisotropicSampler, In.UV).x * 0.25f * (10.0f - NoiseTime);
    //}
    
    
    if (color.a <= 0.0f)
        discard;
    
    if (cbbBool3)
    {
        color *= cbxyzw1; // 곱할 색
    }
    //color += cbxyzw2; // 더할 색
    
    LightColor lightColor = (LightColor) 0.0f;
    for (unsigned int i = 0; i < lightCount; i++)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    color *= lightColor.diffuse;
    
    //color.a = 0.5f;
    //color = defaultTexture.Sample(anisotropicSampler, In.UV);
    return color;
}
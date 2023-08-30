#include "global.hlsli"


struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

//cbuffer time
//int 0
    
float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    //color.a += time;
    
    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (LT - diff - offset) + (atlasSize * In.UV);
        
        if (UV.x < LT.x || UV.y < LT.y || UV.x > LT.x + spriteSize.x || UV.y > LT.y + spriteSize.y)
            discard;
         
        //UV.x = -UV.x;
        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else
    {
        //UV.x = -UV.x;
        //color = defaultTexture.Sample(anisotropicSampler, In.UV);
    }
    
    return color;
}
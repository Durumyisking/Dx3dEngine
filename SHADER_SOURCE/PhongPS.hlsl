#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;
    float  Intensity : FOG;
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    LightColor lightColor = (LightColor) 0.0f;
    
    for (int i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, vsIn.ViewNormal, i, lightColor);
    }
    
    outColor.rgb *= lightColor.diffuse.rgb;

    outColor.rgb += lightColor.specular.rgb;
    
    outColor.rgb += outColor.rgb * lightColor.ambient.rgb;
    
    return outColor;
        
}
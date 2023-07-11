#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
    nointerpolation float3 ViewNormal : NORMAL;
    float intensity : FOG;

};


float4 main(VSOut vsIn) : SV_Target
{ 
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

    LightColor lightColor = (LightColor)0.0f;

    for (int i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, vsIn.ViewNormal, i, lightColor);
    }

    outColor.rgb *= lightColor.diffuse.rgb;

    outColor.rgb += lightColor.specular.rgb;

    outColor.rgb += outColor.rgb * lightColor.ambient.rgb;
    
    return outColor;
}
#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;

};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION;
  
    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    float3 ViewBiNormal : BINORMAL;
    
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float3 normal = vsIn.ViewNormal;

    if (bAlbedo == 1)
    {
        outColor = colorTexture.Sample(anisotropicSampler, vsIn.UV);
        //outColor = TextureMapping_albedo(vsIn.UV);
    }
    if (bNormal == 1)
    {
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    
    LightColor lightColor = (LightColor) 0.0f;
    
    for (uint i = 0; i < lightCount; i++)
    {
        CalculateLight3D(vsIn.ViewPos, normal.xyz, i, lightColor);
    }
    
    outColor = CombineLights(outColor, lightColor);
    
    //if (outColor.w == 0)
    //    discard;
    
    //if (cbxyzw1.w != 0)
    //{
    //    outColor *= cbxyzw1; // 곱할 색        
    //}
    
    //if (cbxyzw2.w != 0)
    //{
    //    outColor += cbxyzw2; // 더할 색    
    //}
        
    
    return outColor;
        
}
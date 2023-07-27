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
    float3 ViewNormal : NORMAL;

    float3 ViewTangent : TANGENT;
    float3 ViewBiNormal : BINORMAL;
};


//diffuse
//specular
//ambient

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

// Material Default Texture
//Texture2D albedoTexture : register(t0);
//Texture2D normalTexture : register(t1);


float4 main(VSOut In) : SV_Target
{
    float4 OutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float3 vNormal = In.ViewNormal;
    
    if (cbtextureExistence == 1)
    {
        OutColor = colorTexture.Sample(anisotropicSampler, In.UV);
    }
    
    
    else if (cbtextureExistence == 2)
    {
    // ��ü�� ǥ�鿡 ����� ź��Ʈ ���� ���� ���⺤�͸� �����´�.
        vNormal = normalTexture.Sample(anisotropicSampler, In.UV);
        
    // 0~1���� -1~1�� ������ ��ȯ
        vNormal = (vNormal * 2.0f) - 1.0f;
        
        float3x3 matTBN =
        {
            In.ViewTangent,
          In.ViewBiNormal,
          In.ViewNormal,
        };

        vNormal = normalize(mul(vNormal, matTBN));
    }
    
    
    LightColor lightColor = (LightColor) 0.0f;
    for (int i = 0; i < lightCount; i++)
    {
        CalculateLight3D(In.ViewPos, vNormal, i, lightColor);
    }
    
    OutColor.rgb = (OutColor.rgb * lightColor.diffuse.rgb
                    + lightColor.specular.rgb 
                    + (OutColor.xyz * lightColor.ambient.rgb));
    
    return OutColor;
        
}
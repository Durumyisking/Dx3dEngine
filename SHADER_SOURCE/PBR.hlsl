#include "global.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION; // viewspace fragmentPos
  
    float3 ViewTangent : TANGENT; // view Space tangent
    float3 ViewNormal : NORMAL; // view Space normal
    float3 ViewBiNormal : BINORMAL; // view Space binormal
    
};


float4 main(VSOut vsIn) : SV_Target
{
    float4  outColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    float4  albedo = float4(0.5f, 0.5f, 0.5f, 1.f);
    float3  normal = vsIn.ViewNormal;    
    float   metallic = 0.01f;
    float   roughness = 0.5f;
    float3  A0 = (float3) 1.f;
    
    int textureCounts = cbtextureExistence;
    
    if (0 == cbtextureExistence)
    {
        normal.xyz = vsIn.ViewNormal;
    }
    else if (1 == cbtextureExistence)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal.xyz = vsIn.ViewNormal;
    }
    else if (2 == cbtextureExistence)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    else if (3 == cbtextureExistence) // metal �߰��ʿ�
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
        metallic = TextureMapping_metallic(vsIn.UV);
    }
    else if (4 == cbtextureExistence)// roughness �߰��ʿ�
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
        metallic = TextureMapping_metallic(vsIn.UV);
        roughness = TextureMapping_roughness(vsIn.UV);
    }
    else if (5 == cbtextureExistence)// emissive �߰��ʿ�
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }

    
    // PBR     
    float3 V        = normalize(vsIn.ViewPos);      // ��
    float3 N        = normal;                       // ����/�ؽ�ó �븻
    float NDotV     = saturate(dot(N, V));          // ��� to �� �ݻ簢
    //float R = 2.0 * NDotV * N - V; // 
    
    for (uint i = 0; i < lightCount; ++i)
    {
        float3 L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // �� ����

        float3 H    = normalize(L + V);                  // �������� : ��, �� �ݻ纤��
  
        float NDotL = saturate(dot(N, L));               // ǥ�� �ݻ� ����
        float NDotH = saturate(dot(N, H));               // ��� �������� ���� (�������Ϳ� ��ġ�Ҽ��� 1�� ���´ٴ°Ϳ��� �ǹ��ִ� ���̴�)
        float VDotH = saturate(dot(V, H));               // �� �������� ����
        float3 F    = FresnelSchlick(albedo.xyz, metallic, V, N);
        float D     = NormalDistributionGGXTR(N, H, roughness);
        float G     = GeometrySmith(N, H, L, roughness);
 
 
        float3 kd   = lerp(float3(1, 1, 1) - F, float3(0.0f, 0.0f, 0.0f), metallic);
 
 
        // Lambert diffuse BRDF.
        float3 diffuseBRDF = kd * albedo.xyz;

        // Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(0.00001f, 4.0f * NDotL * NDotV);
 
//        outColor.xyz += saturate((diffuseBRDF + specularBRDF) * NDotL);
        outColor.xyz += saturate((diffuseBRDF) * NDotL);
 
    }

    //if (outColor.w == 0)
    //    discard;
    
    //if (cbxyzw1.w != 0)
    //{
    //    outColor *= cbxyzw1; // ���� ��        
    //}
    
    //if (cbxyzw2.w != 0)
    //{
    //    outColor += cbxyzw2; // ���� ��    
    //}
        
    
    return outColor;
        
}
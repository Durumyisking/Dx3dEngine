#include "global.hlsli"
#include "BRDF.hlsli"

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION; // viewspace fragmentPos
  
    float3 ViewTangent : TANGENT; // view Space tangent
    float3 ViewNormal : NORMAL; // view Space normal
    float3 ViewBiNormal : BINORMAL; // view Space binormal
      
};

static const float Epsilon = 0.00001;
static const float3 Fdielectric = 0.04;

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = (float4) 0.f;
    outColor.w = 1.f;
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
    else if (3 == cbtextureExistence) 
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
        metallic = TextureMapping_metallic(vsIn.UV);
    }
    else if (4 == cbtextureExistence)
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
    float3 V = normalize(-vsIn.ViewPos); // ����� pinPoint(0,0,0)���� �ȼ��� ���ϴ� ����
    float3 N = normal; // ����/�ؽ�ó �븻 �亯ȯ �Ϸ�
    float NDotV = saturate(dot(N, V)); // ��� to �� �ݻ簢 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // �ݼӼ��� ���Ҽ��� albedo�� ����ϰ� �ƴϸ� 0.04��� (���� ��)


    
    for (uint i = 0; i < lightCount; ++i)
    {
        float3 Lradiance = lightAttributes[i].color.diffuse.xyz;
        float3 L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // �� ����

        float3 H = normalize(L + V); // �������� : ��, �� �ݻ纤��
  
        float NDotL = saturate(dot(N, L)); // ǥ�� �ݻ� ����
        float NDotH = saturate(dot(N, H)); // ��� �������� ���� (�������Ϳ� ��ġ�Ҽ��� 1�� ���´ٴ°Ϳ��� �ǹ��ִ� ���̴�)
        float VDotH = saturate(dot(V, H)); // �� �������� ����
        float3 F = FresnelSchlick(F0, VDotH); // �� �ݻ���  metallic�� �������� ���� ���� �ݻ��Ѵ�.
        float D = NormalDistributionGGXTR(NDotH, roughness); // ǥ���� ��ħ ������ �����, �̼�ǥ���� ��ĥ��� �츮�� ��Ȯ�� �� �� ���⶧���� ����ġ�� ���Ѵ�. roughness�� �������� ���� ���� �ݻ��Ѵ�.
        float G = GeometrySmith(NDotL, NDotV, roughness); // �׵θ� ��ħ���� �⿩�Ѵ�. metallic�� �������� view�� �븻���Ͱ� �̷�� ������ Ŭ���� ���� ����ϱ⺸�ٴ� �ݻ��Ѵ�.
 
 
        // ���� ������ ������ ������ �����Ǿ� Ȯ�� ����� �Ͼ��.
        // �ݼӼ��� �������� �������� �ݻ��ϰų� ����ϱ⶧���� Ȯ��⿩�� ����        
        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic); // f�� sub �ϸ� ��� �������� ����

        // Lambert diffuse BRDF.
        float3 diffuseBRDF = kd * albedo.xyz;
        
        // Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = ((F * G) / max(Epsilon, 4.0f * NDotL * NDotV));

        outColor.xyz += saturate((diffuseBRDF + specularBRDF) * Lradiance * NDotL); // NDotL�� ������ ������ diffuse�� ��� �鿡 �ۿ��Ѵ�.

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
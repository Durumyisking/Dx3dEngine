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

static const float Epsilon = 0.00001f;
static const float3 Fdielectric = 0.04f;

float4 main(VSOut vsIn) : SV_Target
{
    float3 outColor = (float3) 0.f;
    
    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;

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
    normal.xyz = vsIn.ViewNormal;

    // PBR     
    float3 V = normalize(-vsIn.ViewPos); // ����� pinPoint(0,0,0)���� �ȼ��� ���ϴ� ����
    float3 N = normal; // ����/�ؽ�ó �븻 �亯ȯ �Ϸ�
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // ��� to �� �ݻ簢 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // �ݼӼ��� ���Ҽ��� albedo�� ����ϰ� �ƴϸ� 0.04��� (���� ��)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // �� ����            

    float NdotL = max(dot(N, L), 0.f);
    
    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);
    
    float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);

    //float3 irradiance = irradianceMap.Sample(splr, N).rgb;
    float3 diffuse = kd * albedo.xyz;

    const float MAX_REFLECTION_LOD = 4.f;
    //float3 prefilteredColor = prefilteredMap.SampleLevel(splr, R, roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.f), roughness)).rg;
    float3 specular = (F * envBRDF.x + envBRDF.y);
        
    float3 ambient = (diffuse + specular) * NdotL;
    
    outColor.xyz = ambient;   
        
    return float4(outColor, 1.f);;
    
}











//    for (uint i = 0; i < lightCount; ++i)
//    {
//        float3 Lradiance = lightAttributes[i].color.diffuse.xyz;
//        float3 L = (float3) 0.f;

//        if (0 == lightAttributes[i].type)
//        {
//            L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // �� ����            
//        }
//        else if (1 == lightAttributes[i].type)
//        {
//            float3 lightViewPos = mul(float4(lightAttributes[i].position.xyz, 1.f), view).xyz;
//            L = vsIn.ViewPos - lightViewPos;
//        }
        
        
//        float3 H = normalize(V + L); // �������� : ��, �� �ݻ纤��
        
//        float3 F = fresnelSchlick(max(dot(H, V), 0.f), F0); // �� �ݻ���  metallic�� �������� ���� ���� �ݻ��Ѵ�.
//        float D = DistributionGGX(N, H, roughness); // ǥ���� ��ħ ������ �����, �̼�ǥ���� ��ĥ��� �츮�� ��Ȯ�� �� �� ���⶧���� ����ġ�� ���Ѵ�. roughness�� �������� ���� ���� �ݻ��Ѵ�.
//        float G = GeometrySmith(N, V, L, roughness); // �׵θ� ��ħ���� �⿩�Ѵ�. metallic�� �������� view�� �븻���Ͱ� �̷�� ������ Ŭ���� ���� ����ϱ⺸�ٴ� �ݻ��Ѵ�.
 
 
//        // ���� ������ ������ ������ �����Ǿ� Ȯ�� ����� �Ͼ��.
//        // �ݼӼ��� �������� �������� �ݻ��ϰų� ����ϱ⶧���� Ȯ��⿩�� ����        
//        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
//        // Lambert diffuse BRDF.
//        float3 diffuseBRDF = kd * albedo.xyz;
        
//        // Cook-Torrance specular microfacet BRDF.
////        float3 specularBRDF = ((F * D * G) / max(Epsilon, 4.f * NDotL * NDotV));
//        float3 specNumerator = D * G * F;
//        float specDenominator = 4.f * max(dot(N, V), 0.f) * max(dot(N, L), 0.f) + 0.0001f;
//        float3 specularBRDF = specNumerator / specDenominator;
//        float NdotL = max(dot(N, L), 0.f);
        
//        directLighting += saturate((diffuseBRDF + specularBRDF) * Lradiance * NdotL); // NDotL�� ������ ������ diffuse�� ��� �鿡 �ۿ��Ѵ�.

//    }
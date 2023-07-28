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
    else if (5 == cbtextureExistence)// emissive 추가필요
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    normal.xyz = vsIn.ViewNormal;

    // PBR     
    float3 V = normalize(-vsIn.ViewPos); // 뷰공간 pinPoint(0,0,0)부터 픽셀로 향하는 벡터
    float3 N = normal; // 정점/텍스처 노말 뷰변환 완료
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // 노멀 to 눈 반사각 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // 금속성이 강할수록 albedo를 사용하고 아니면 0.04사용 (재질 값)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // 빛 각도            

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
//            L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // 빛 각도            
//        }
//        else if (1 == lightAttributes[i].type)
//        {
//            float3 lightViewPos = mul(float4(lightAttributes[i].position.xyz, 1.f), view).xyz;
//            L = vsIn.ViewPos - lightViewPos;
//        }
        
        
//        float3 H = normalize(V + L); // 하프벡터 : 눈, 빛 반사벡터
        
//        float3 F = fresnelSchlick(max(dot(H, V), 0.f), F0); // 빛 반사율  metallic이 높을수록 빛을 많이 반사한다.
//        float D = DistributionGGX(N, H, roughness); // 표면의 거침 분포를 계산함, 미세표면의 거칠기는 우리가 정확히 알 수 없기때문에 추정치를 구한다. roughness가 낮을수록 빛을 세게 반사한다.
//        float G = GeometrySmith(N, V, L, roughness); // 테두리 거침도에 기여한다. metallic한 물질들은 view와 노말벡터가 이루는 각도가 클수록 빛을 흡수하기보다는 반사한다.
 
 
//        // 빛은 물질에 닿으면 여러번 굴절되어 확산 산란이 일어난다.
//        // 금속성이 높을수록 에너지를 반사하거나 흡수하기때문에 확산기여는 낮다        
//        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
//        // Lambert diffuse BRDF.
//        float3 diffuseBRDF = kd * albedo.xyz;
        
//        // Cook-Torrance specular microfacet BRDF.
////        float3 specularBRDF = ((F * D * G) / max(Epsilon, 4.f * NDotL * NDotV));
//        float3 specNumerator = D * G * F;
//        float specDenominator = 4.f * max(dot(N, V), 0.f) * max(dot(N, L), 0.f) + 0.0001f;
//        float3 specularBRDF = specNumerator / specDenominator;
//        float NdotL = max(dot(N, L), 0.f);
        
//        directLighting += saturate((diffuseBRDF + specularBRDF) * Lradiance * NdotL); // NDotL을 곱하지 않으면 diffuse가 모든 면에 작용한다.

//    }
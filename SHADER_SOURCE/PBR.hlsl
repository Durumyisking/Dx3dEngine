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
    else if (5 == cbtextureExistence)// emissive 추가필요
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }

 
    // PBR     
    float3 V = normalize(-vsIn.ViewPos); // 뷰공간 pinPoint(0,0,0)부터 픽셀로 향하는 벡터
    float3 N = normal; // 정점/텍스처 노말 뷰변환 완료
    float NDotV = saturate(dot(N, V)); // 노멀 to 눈 반사각 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // 금속성이 강할수록 albedo를 사용하고 아니면 0.04사용 (재질 값)


    
    for (uint i = 0; i < lightCount; ++i)
    {
        float3 Lradiance = lightAttributes[i].color.diffuse.xyz;
        float3 L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // 빛 각도

        float3 H = normalize(L + V); // 하프벡터 : 눈, 빛 반사벡터
  
        float NDotL = saturate(dot(N, L)); // 표면 반사 각도
        float NDotH = saturate(dot(N, H)); // 노멀 하프벡터 각도 (하프벡터와 일치할수록 1이 나온다는것에서 의미있는 값이다)
        float VDotH = saturate(dot(V, H)); // 눈 하프벡터 각도
        float3 F = FresnelSchlick(F0, VDotH); // 빛 반사율  metallic이 높을수록 빛을 많이 반사한다.
        float D = NormalDistributionGGXTR(NDotH, roughness); // 표면의 거침 분포를 계산함, 미세표면의 거칠기는 우리가 정확히 알 수 없기때문에 추정치를 구한다. roughness가 낮을수록 빛을 세게 반사한다.
        float G = GeometrySmith(NDotL, NDotV, roughness); // 테두리 거침도에 기여한다. metallic한 물질들은 view와 노말벡터가 이루는 각도가 클수록 빛을 흡수하기보다는 반사한다.
 
 
        // 빛은 물질에 닿으면 여러번 굴절되어 확산 산란이 일어난다.
        // 금속성이 높을수록 에너지를 반사하거나 흡수하기때문에 확산기여는 낮다        
        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic); // f를 sub 하면 가운데 점같은거 생김

        // Lambert diffuse BRDF.
        float3 diffuseBRDF = kd * albedo.xyz;
        
        // Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = ((F * G) / max(Epsilon, 4.0f * NDotL * NDotV));

        outColor.xyz += saturate((diffuseBRDF + specularBRDF) * Lradiance * NDotL); // NDotL을 곱하지 않으면 diffuse가 모든 면에 작용한다.

    }
    
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
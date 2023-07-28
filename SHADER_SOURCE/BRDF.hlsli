float PI = 3.1415926535f;

// microfacet 계산       
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.f);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return num / denom;
}


// 에너지 보존법칙 계산
// 거칠기에 따른 쉐도잉과 마스킹 연산
// 거칠기 up = 마스킹 쉐도잉 up
// 미세면이 거칠면 우리 눈에 들어올 확률이 낮아짐   
// Schlick-GGX로 알려진 Schlick-Beckmann 근사

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.f);
    float k = (r * r) / 8.f;

    float num = NdotV;
    float denom = NdotV * (1.f - k) + k;

    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);  // 빛의 방향에 대한 그림자 Geometric Occlusion 반사 ( 미세표면에 의해 카메라가 보는 라이트 소스가 가려지는 영역)
    float ggx1 = GeometrySchlickGGX(NdotL, roughness); // 시야 방향에 대한 그림자 Geometric Shadowing 입사 ( 미세표면에 의해 빛이 표면에 도달하지 못하는 영역)

    return ggx1 * ggx2;
}


// Fresnel 광택 계산 함수                             
// 반사율을 근사적으로 계산한다.
// cosTheta (빛의 입사각) 
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.f - F0) * pow(1.f - cosTheta, 5.f);
}
// 러프니스까지 고려한 프레넬 계산함수라고한다.
float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.f - roughness, 1.f - roughness, 1.f - roughness), F0) - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}



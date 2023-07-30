float PI = 3.1415926535f;

// microfacet 계산        // 미세표면과 halvvector
float NormalDistributionGGXTR(float NDotH, float roughness)
{
    float roughness2 = roughness * roughness;
    float NdotH2 = NDotH * NDotH;

    // GGX Trowbridge-Reitz                             // microfacet normal
    float denom = (NdotH2 * (roughness2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return roughness2 / denom;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// 에너지 보존법칙 계산
// 거칠기에 따른 쉐도잉과 마스킹 연산
// 거칠기 up = 마스킹 쉐도잉 up
// 미세면이 거칠면 우리 눈에 들어올 확률이 낮아짐   
// Schlick-GGX로 알려진 Schlick-Beckmann 근사
float GeometrySchlickGGX(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.f - k) + k);
}
  
float GeometrySmith(float NDotL, float NDotV, float roughness)
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f;

    float ggx1 = GeometrySchlickGGX(NDotL, k); // 빛의 방향에 대한 그림자 Geometric Occlusion 반사 ( 미세표면에 의해 카메라가 보는 라이트 소스가 가려지는 영역)
    float ggx2 = GeometrySchlickGGX(NDotV, k); // 시야 방향에 대한 그림자 Geometric Shadowing 입사 ( 미세표면에 의해 빛이 표면에 도달하지 못하는 영역)

    return ggx1 * ggx2;
}

// Fresnel 광택 계산 함수                             
// 반사율을 근사적으로 계산한다.
// cosTheta (빛의 입사각) 
float3 FresnelSchlick(float3 F0, float VDotH)
{
    // Fresnel Schlick 
    float3 fresnel = F0 + (1.f - F0) * pow(1.f - VDotH, 5.f);

    return fresnel;
}



// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}


// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

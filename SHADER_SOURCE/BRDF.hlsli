float PI = 3.1415926535f;

// microfacet ���        // �̼�ǥ��� halvvector
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

// ������ ������Ģ ���
// ��ĥ�⿡ ���� �����װ� ����ŷ ����
// ��ĥ�� up = ����ŷ ������ up
// �̼����� ��ĥ�� �츮 ���� ���� Ȯ���� ������   
// Schlick-GGX�� �˷��� Schlick-Beckmann �ٻ�
float GeometrySchlickGGX(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.f - k) + k);
}
  
float GeometrySmith(float NDotL, float NDotV, float roughness)
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f;

    float ggx1 = GeometrySchlickGGX(NDotL, k); // ���� ���⿡ ���� �׸��� Geometric Occlusion �ݻ� ( �̼�ǥ�鿡 ���� ī�޶� ���� ����Ʈ �ҽ��� �������� ����)
    float ggx2 = GeometrySchlickGGX(NDotV, k); // �þ� ���⿡ ���� �׸��� Geometric Shadowing �Ի� ( �̼�ǥ�鿡 ���� ���� ǥ�鿡 �������� ���ϴ� ����)

    return ggx1 * ggx2;
}

// Fresnel ���� ��� �Լ�                             
// �ݻ����� �ٻ������� ����Ѵ�.
// cosTheta (���� �Ի簢) 
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

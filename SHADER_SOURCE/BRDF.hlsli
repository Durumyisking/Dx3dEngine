float PI = 3.1415926535f;

// microfacet ���       
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


// ������ ������Ģ ���
// ��ĥ�⿡ ���� �����װ� ����ŷ ����
// ��ĥ�� up = ����ŷ ������ up
// �̼����� ��ĥ�� �츮 ���� ���� Ȯ���� ������   
// Schlick-GGX�� �˷��� Schlick-Beckmann �ٻ�

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
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);  // ���� ���⿡ ���� �׸��� Geometric Occlusion �ݻ� ( �̼�ǥ�鿡 ���� ī�޶� ���� ����Ʈ �ҽ��� �������� ����)
    float ggx1 = GeometrySchlickGGX(NdotL, roughness); // �þ� ���⿡ ���� �׸��� Geometric Shadowing �Ի� ( �̼�ǥ�鿡 ���� ���� ǥ�鿡 �������� ���ϴ� ����)

    return ggx1 * ggx2;
}


// Fresnel ���� ��� �Լ�                             
// �ݻ����� �ٻ������� ����Ѵ�.
// cosTheta (���� �Ի簢) 
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.f - F0) * pow(1.f - cosTheta, 5.f);
}
// �����Ͻ����� ����� ������ ����Լ�����Ѵ�.
float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.f - roughness, 1.f - roughness, 1.f - roughness), F0) - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}



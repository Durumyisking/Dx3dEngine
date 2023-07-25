#include "Random.hlsli"

struct LightColor
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
    
};

struct LightAttribute
{
    LightColor color;
    float4 position;
    float4 direction;
    
    float radius;
    float angle;    

    int type;
    
    int padding;

};


StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

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



void CalculateLight(in out LightColor lightColor, float3 position, int idx) // in out Ű����� ����/�����ͷ� ������
{
    if (0 == lightAttributes[idx].type)
    {
        lightColor.diffuse += lightAttributes[idx].color.diffuse;
        
    }
    else if (1 == lightAttributes[idx].type)
    {
        float dist = distance(lightAttributes[idx].position.xy, position.xy); // z���� ����������.
        
        if (dist < lightAttributes[idx].radius)
        {
            float ratio = 1.f - (dist / lightAttributes[idx].radius);
            lightColor.diffuse += lightAttributes[idx].color.diffuse * ratio;
        }
    } 
}

void CalculateLight3D(float3 viewPos, float3 viewNormal, int lightIdx, inout LightColor lightColor)
{
    LightAttribute lightInfo = lightAttributes[lightIdx];
    
    float3 viewLightDir = (float3) 0.f;

    //view space �󿡼� ���� ���⸦ ����
    float diffseIntensity = 0.f;
    float specularIntensity = 0.f;
    float3 eye = (float3) 0.f;
    float3 viewReflect = (float3) 0.f;
    
    // Directional
    if (0 == lightInfo.type)
    {
        // ������ ������ ���� ��ǥ�迡�� �� ��ǥ��� ��ȯ
        // direction�� lightObject�� transform�� forward �Դϴ�.
        viewLightDir = normalize(mul(float4(lightInfo.direction.xyz, 0.f), view)).xyz;

        // ���⿡ ������ ���ϴ� ����
        // ���� ǥ�鿡 ǥ���ϰ� �ͱ⶧�� (������ ������ �������� 1�� �������)
        // ������ ������ ������ ���� ����� normal������ ���� 180���� �Ѿ�� �� �� ���ʿ� ���� ��
        diffseIntensity = saturate(dot(-viewLightDir, viewNormal));
        
    }
    // point
    else if (1 == lightInfo.type)
    {
        // �� ��ǥ�� �󿡼� ������ ��ġ�� �˾Ƴ���.
        // point light�� �� object �ֺ����� ���� ����ϱ� ������ �������� direction���� ������� �ʴ´�.
        // ǥ������� ������ ���� ��������Ѵ�.
        float3 lightViewPos = mul(float4(lightInfo.position.xyz, 1.f), view).xyz;
        
        // ������ ��ġ���� ǥ���� ���ϴ� ����
        viewLightDir = viewPos - lightViewPos;
        
        //�������� ǥ������� �Ÿ��� ���Ѵ�.
        float dist = length(viewLightDir);
        
        // �������� ǥ���� ���ϴ� �������͸� ���Ѵ�.
        viewLightDir = normalize(viewLightDir);
        
        // ���� ����
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535 * 0.5f);
        
        //view space �󿡼� ǥ���� ���� ���⸦ ����
        diffseIntensity = saturate(dot(-viewLightDir, viewNormal)) * ratio;
        
    }
    else
    {
        
    }
    // �ݻ籤 ���⸦ ����
    // ǥ���� ���� �ݻ纤��
    viewReflect = normalize(viewLightDir + 2.f * dot(-viewLightDir, viewNormal) * viewNormal);
        
    // �������� ǥ���� ���ϴ� ����
    eye = normalize(viewPos);
    
     //�ü� ���Ͷ� �ݻ纤�͸� �����ؼ� �ݻ籤�� ���⸦ ���Ѵ�.
    specularIntensity = saturate(dot(-eye, viewReflect));
    specularIntensity = pow(specularIntensity, 30);
    
    // ���� ���ݻ籤
    lightColor.diffuse += lightInfo.color.diffuse * diffseIntensity;
        
    // ���ݻ籤
    lightColor.specular += lightInfo.color.specular * specularIntensity;
        
    // �ֺ���
    lightColor.ambient = lightInfo.color.ambient;
}

float3 CalculateLight3D_PBR(float4 albedo, float3 viewNormal, float metallic, float roughness, float3 viewPos, int lightIdx, inout LightColor lightColor)
{
    return (float3) 0.f;
}


float4 CombineLights(float4 color, LightColor lightColor)
{
    color.rgb *= lightColor.diffuse.rgb;

    color.rgb += lightColor.specular.rgb;
    
    color.rgb += color.rgb * lightColor.ambient.rgb;

    return color;
}



//3D
static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);
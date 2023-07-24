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

// microfacet ���
float NormalDistributionGGXTR(float3 normalDir, float3 halfVector, float roughness)
{
    float roughness2 = roughness * roughness;
    float NdotH = saturate(dot(normalDir, halfVector)); // normal�� halfvec�� ������ ���Ѵ�.
    float NdotH2 = NdotH * NdotH;

    // GGX Trowbridge-Reitz                             // microfacet normal
    float denom = (NdotH2 * (roughness2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return roughness2 / denom;
}

// ������ ������Ģ ���
// ��ĥ�⿡ ���� �����װ� ����ŷ ����
// ��ĥ�� up = ����ŷ ������ up

float GeometrySchlickGGX(float cosTheta, float roughness)  
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f; 
    // k �� ibl ������ �̳� directional light�� ���� �ٸ��� ��ġ�ȴ�.

    //float k = (roughness * roughness) / 2.0f;

    float nom = cosTheta;
    float denom = cosTheta * (1.f - k) + k;

    return nom / denom;
}
  
float GeometrySmith(float3 normalDir, float3 halfVector, float3 lightDir, float roughness)
{
    //  Geometry Obstruction
    float NdotV = saturate(dot(normalDir, lightDir));
    //  Geometry Shadowing
    float NdotL = saturate(dot(normalDir, halfVector));

    float ggx1 = GeometrySchlickGGX(NdotV, roughness); // ���� ���⿡ ���� �׸��� Geometric Occlusion �ݻ� ( �̼�ǥ�鿡 ���� ī�޶� ���� ����Ʈ �ҽ��� �������� ����)
    float ggx2 = GeometrySchlickGGX(NdotL, roughness); // �þ� ���⿡ ���� �׸��� Geometric Shadowing �Ի� ( �̼�ǥ�鿡 ���� ���� ǥ�鿡 �������� ���ϴ� ����)

    return ggx1 * ggx2;
}

// Fresnel ���� ��� �Լ�                             
float3 FresnelSchlick(float3 albedo, float metallic, float3 eyeDir, float3 normalDir)
{
                    // Fresnel ���� ���    
    float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);
    
    float VDotH = saturate(dot(eyeDir, normalDir)); // �� �Ի纤�Ϳ� ǥ�� normal ������ �� 1�� �������� ���� �Ի�

    // Fresnel Schlick 
    float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - VDotH, 5.0);

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
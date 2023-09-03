#include "Random.hlsli"
#include "BRDF.hlsli"
#include "Texture.hlsli"
#include "Sampler.hlsli"

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


StructuredBuffer<LightAttribute> lightAttributes : register(t22);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

static const float Epsilon = 0.00001f;
static const float3 Fdielectric = 0.04f;


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
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535f * 0.5f);
        
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

float3 CalculateLightPBR_Direct(float3 viewPos, float4 albedo, float3 viewNormal, float metallic, float roughness, float3 uv)
{
    // PBR     
    float3 V = normalize(viewPos); // ����� pinPoint(0,0,0)���� �ȼ��� ���ϴ� ����
    float3 N = normalize(viewNormal); // ����/�ؽ�ó �븻 �亯ȯ �Ϸ�
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // ��� to �� �ݻ簢 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // �ݼӼ��� ���Ҽ��� albedo�� ����ϰ� �ƴϸ� 0.04��� (���� ��)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // �� ����            

    float NdotL = max(dot(N, L), 0.f);

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);

    float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
    //float3 ks = F;
    //float3 kd = 1.f - ks;
    //kd *= 1.f - metallic;

    float3 irradiance = irradianceMap.Sample(linearSampler, N).rgb;
    float3 diffuse = irradiance *  albedo.xyz;

    const float MAX_REFLECTION_LOD = 4.f;
    float3 prefilteredColor = prefilteredMap.SampleLevel(linearSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.f), roughness)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y); 

    float3 result = (diffuse + specular) * NdotL;
    //float3 result = (kd * diffuse + specular); // �������̴� ������ �� ���ߴµ�?4
    
    return result;
}

float3 CalculateLightPBR_Diffuse(float3 viewPos, float4 albedo, float3 viewNormal, float metallic, float roughness)
{
    // PBR     
    float3 V = normalize(viewPos);
    float3 N = viewNormal;
    float NDotV = saturate(dot(N, V));

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); 

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz;          

    float NdotL = max(dot(N, L), 0.f);

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);

    float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);

    //float3 irradiance = irradianceMap.Sample(linearSampler, N).rgb;
    //float3 diffuse = irradiance * kd * albedo.xyz;
    float3 diffuse = kd * albedo.xyz;

    return diffuse;
}
float3 CalculateLightPBR_Specular(float3 viewPos, float4 albedo, float3 viewNormal, float metallic, float roughness)
{
    // PBR     
    float3 V = normalize(-viewPos); // ����� pinPoint(0,0,0)���� �ȼ��� ���ϴ� ����
    float3 N = viewNormal; // ����/�ؽ�ó �븻 �亯ȯ �Ϸ�
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // ��� to �� �ݻ簢 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // �ݼӼ��� ���Ҽ��� albedo�� ����ϰ� �ƴϸ� 0.04��� (���� ��)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // �� ����            

    float NdotL = max(dot(N, L), 0.f);

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);
    
    const float MAX_REFLECTION_LOD = 4.f;
    float3 prefilteredColor = prefilteredMap.SampleLevel(linearSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.f), roughness)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    //float3 specular = (F * envBRDF.x + envBRDF.y);
    
    return specular;
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
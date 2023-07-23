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
float NormalDistributionGGXTR(float3 normal, float3 viewReflectDir, float roughness)
{
    float roughness2 = roughness * roughness;
    float NdotH = saturate(dot(normal, viewReflectDir)); // normal�� halfvec�� ������ ���Ѵ�.
    float NdotH2 = NdotH * NdotH;

    float nom = roughness2;
    float denom = (NdotH2 * (roughness2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

// ������ ������Ģ ���
// ��ĥ�⿡ ���� �����װ� ����ŷ ����
// ��ĥ�� up = ����ŷ ������ up

float GeometrySchlickGGX(float NdotV, float roughness)  
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f; 
    // k �� ibl ������ �̳� directional light�� ���� �ٸ��� ��ġ�ȴ�.

    //float k = (roughness * roughness) / 2.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}
  
float GeometrySmith(float3 normal, float3 eyeDir, float3 viewLightDir, float roughness)
{
    //  Geometry Obstruction
    float NdotV = saturate(dot(normal, -eyeDir));
    //  Geometry Shadowing
    float NdotL = saturate(dot(normal, viewLightDir));

    float ggx1 = GeometrySchlickGGX(NdotV, roughness); // �þ� ���⿡ ���� �׸���
    float ggx2 = GeometrySchlickGGX(NdotL, roughness); // ���� ���⿡ ���� �׸���

    return ggx1 * ggx2;
}

// Fresnel ���� ��� �Լ�                             // eyedir
float3 FresnelSchlick(float3 albedo, float metallic, float3 viewDir, float3 viewReflectDir)
{
    // Fresnel ���� ���
    float3 F0 = (float3) 0.4f; // �����ΰ���
    
    F0 = lerp(F0, albedo, metallic);

    // Fresnel Schlick �ٻ�
    viewDir = normalize(viewDir);
    viewReflectDir = normalize(viewReflectDir);
    
    float VoH = saturate(dot(viewDir, normalize(viewReflectDir))); // �� �Ի纤�Ϳ� ǥ�� normal ������ �� 1�� �������� ���� �Ի�
    float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - VoH, 5.0);

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
    LightAttribute lightInfo = lightAttributes[lightIdx];
    
    float3 directLighting = (float3) 0.f;
    
    float3 lightDir         = lightInfo.direction.xyz;
    float3 viewLightDir     = (float3) 0.f;
    float3 viewReflectDir   = (float3) 0.f;

    float3 eyeDir           = normalize(viewPos);
    
    float3 radiance         = lightInfo.color.diffuse.xyz;

    // Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(viewNormal, eyeDir)); // ī�޶� dir�� , �������Ϳ��� 0 �̻��� ������ ��ȯ
		
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * viewNormal - eyeDir;

    // Directional
    if (0 == lightInfo.type)
    {
        // ������ ������ ���� ��ǥ�迡�� �� ��ǥ��� ��ȯ
        // direction�� lightObject�� transform�� forward �Դϴ�.
        viewLightDir = normalize(mul(float4(lightDir, 0.f), view)).xyz;
               
    	// Half-vector between Li and Lo.
        float3 Lh = normalize(-lightInfo.direction.xyz + eyeDir);
        
        // Calculate angles between surface normal and various light vectors.
        float cosLi = saturate(dot(viewNormal, -lightDir));
        float cosLh = saturate(dot(viewNormal, eyeDir));
        
        float3 F = FresnelSchlick(albedo.xyz, metallic, eyeDir, viewReflectDir);
        float D = NormalDistributionGGXTR(viewNormal, viewReflectDir, roughness);
        float G = GeometrySmith(viewNormal, eyeDir, lightDir, roughness);
        
        float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
        
        // Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
        float3 diffuseBRDF = kd * albedo.xyz;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(0.00001f, 4.f * cosLi * cosLo);

		// Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * radiance * cosLi;
    }
    // point
    else if (1 == lightInfo.type)
    {
        // �� ��ǥ�� �󿡼� ������ ��ġ�� �˾Ƴ���.
        // point light�� �� object �ֺ����� ���� ����ϱ� ������ �������� direction���� ������� �ʴ´�.
        // ǥ������� ������ ���� ��������Ѵ�.
        float3 lightViewPos = mul(float4(lightInfo.position.xyz, 1.f), view).xyz;
        
        // ������ ��ġ���� ǥ���� ���ϴ� ����
        viewLightDir = lightViewPos - viewPos;
        
        //�������� ǥ������� �Ÿ��� ���Ѵ�.
        float dist = length(viewLightDir);
        
        // �������� ǥ���� ���ϴ� �������͸� ���Ѵ�.
        viewLightDir = normalize(viewLightDir);
        
        float cosTheta = max(dot(viewNormal, viewLightDir), 0.0); // ���� ���� ���� ����
        
        // ���� ����
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535 * 0.5f);    
                
        //view space �󿡼� ǥ���� ���� ���⸦ ����
        radiance = lightInfo.color.diffuse.xyz * ratio * cosTheta; // ���� �Ի� ��
    }
    else
    {
        
    }

    // Final fragment color.
    return float4(directLighting, 1.0);
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
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

float3 CalculateLightPBR_Direct(float3 worldPos, float4 albedo, float3 worldNormal, float metallic, float roughness)
{
    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // �� ����

    // PBR     
    float3 N = normalize(worldNormal); 
    float3 V = normalize(cameraWorldPos.xyz - worldPos); 
    float3 R = reflect(-V, N);

    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo.xyz, metallic); // �ݼӼ��� ���Ҽ��� albedo�� ����ϰ� �ƴϸ� 0.04��� (���� ��)

    float NDotV = saturate(dot(N, V)); // ��� to �� �ݻ簢 
    float NdotL = max(dot(N, L), 0.f); // �븻�� ��

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    

    //float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
    float3 ks = F;
    float3 kd = 1.0f - ks;
    kd *= 1.0f - metallic;

    float3 irradiance = irradianceMap.Sample(skyBoxSampler, N).rgb;    
    float3 diffuse = irradiance * albedo.xyz;

    const float MAX_REFLECTION_LOD = 4.f;
    float3 prefilteredColor = prefilteredMap.SampleLevel(skyBoxSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    
    // brdf �ؽ�ó�� x�� �ݻ翡 ���� y�� �����Ͻ��� ���� ���ø��Ѵ�.
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.0), roughness)).rg;
    
                                        // �ݻ� ���õ� x�� �����ڰ� ��� y�� diffuseó�� �����ϴ� �� ����
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    float3 result = (kd * diffuse + specular);// * NdotL;

    return result;
}


float4 CombineLights(float4 color, LightColor lightColor)
{
    color.rgb *= lightColor.diffuse.rgb;

    color.rgb += lightColor.specular.rgb;
    
    color.rgb += color.rgb * lightColor.ambient.rgb;

    return color;
}

float3 DiffuseIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                  float metallic)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = fresnelSchlick(F0, max(0.0, dot(normalWorld, pixelToEye)));
    float3 kd = lerp(1.0 - F, 0.0, metallic);
    float3 irradiance = irradianceMap.SampleLevel(linearSampler, normalWorld, 0).rgb;
    
    return kd * albedo * irradiance;
}

float3 SpecularIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                   float metallic, float roughness, float pixelToCam)
{
    float mip = pixelToCam / 1.2f;
    if(mip > 6)
        mip = 6;
    float2 specularBRDF = BRDF.SampleLevel(clampSampler, float2(dot(normalWorld, pixelToEye), 1.0 - roughness), 0.0f).rg;
    float3 specularIrradiance = prefilteredMap.SampleLevel(linearSampler, reflect(-pixelToEye, normalWorld),
                                                            0.f).rgb;
    const float3 Fdielectric = 0.04; // ��ݼ�(Dielectric) ������ F0
    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}
float3 AmbientLightingByIBL(float3 albedo, float3 normalW, float3 pixelToEye,
                            float metallic, float roughness, float pixelToCam)
{
    float3 diffuseIBL = DiffuseIBL(albedo, normalW, pixelToEye, metallic);
    float3 specularIBL = SpecularIBL(albedo, normalW, pixelToEye, metallic, roughness, pixelToCam);
    
    return (diffuseIBL + specularIBL);
}

float3 LightRadiance(LightAttribute light, float3 posWorld, float3 normalWorld, Texture2D shadowMap)
{
    // Directional light
    //float3 lightVec = light.type & 0
    //                  ? -light.direction
    //                  : light.position.xyz - posWorld;
    float3 lightVec = -light.direction.xyz;
                              
    float lightDist = length(lightVec);
    lightVec /= lightDist;

    float spotFator = 1.0f;
        
    // Distance attenuation
    float att = saturate((20.f - lightDist)
                         / (20.f - 0.f));

    // Shadow map
    float shadowFactor = 1.0;
    
    {
        const float nearZ = 0.01; // ī�޶� ������ ����
        
        // 1. Project posWorld to light screen    
        float4 lightScreen = mul(float4(posWorld, 1.0), lightProjection);
        lightScreen.xyz /= lightScreen.w;
        
        // 2. ī�޶�(����)���� �� ���� �ؽ��� ��ǥ ���
        float2 lightTexcoord = float2(lightScreen.x, -lightScreen.y);
        lightTexcoord += 1.0;
        lightTexcoord *= 0.5;
        
        // 3. ������ʿ��� �� ��������
        float depth = shadowMap.Sample(linearSampler, lightTexcoord).r;
        //float depth = shadowMap.Sample(shadowPointSampler, lightTexcoord).r;
        
        // 4. ������ �ִٸ� �׸��ڷ� ǥ��
        if (depth + 0.001 < lightScreen.z)
            shadowFactor = 0.0;
        
    }
    //              light.radiance
    float3 radiance =    5.f    * spotFator * att * shadowFactor;

    return radiance;
}



float VSM_FILTER(float2 moments, float fragDepth)
{
    float lit = (float) 1.0f;
    float E_x2 = moments.y;
    float Ex_2 = moments.x * moments.x;
    float variance = E_x2 - Ex_2;
    variance = max(variance, 0.0000005f);

    float mD = fragDepth - moments.x;
    
    float p = 1.f;
    if (mD > 0.f)
    {
        float mD_2 = mD * mD;
        p = variance / (variance + mD_2);
    }
        
    lit = max(p, 0.4f);

    return lit;
}


//3D
static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);
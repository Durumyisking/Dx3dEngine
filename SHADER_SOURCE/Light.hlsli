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


void CalculateLight(in out LightColor lightColor, float3 position, int idx) // in out 키워드는 참조/포인터로 쓸거임
{
    if (0 == lightAttributes[idx].type)
    {
        lightColor.diffuse += lightAttributes[idx].color.diffuse;
        
    }
    else if (1 == lightAttributes[idx].type)
    {
        float dist = distance(lightAttributes[idx].position.xy, position.xy); // z값을 연산해주자.
        
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

    //view space 상에서 빛의 세기를 구함
    float diffseIntensity = 0.f;
    float specularIntensity = 0.f;
    float3 eye = (float3) 0.f;
    float3 viewReflect = (float3) 0.f;
    
    // Directional
    if (0 == lightInfo.type)
    {
        // 광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환
        // direction은 lightObject의 transform의 forward 입니다.
        viewLightDir = normalize(mul(float4(lightInfo.direction.xyz, 0.f), view)).xyz;

        // 방향에 음수를 취하는 이유
        // 빛을 표면에 표시하고 싶기때문 (내적은 방향이 같을수록 1에 가까워짐)
        // 음수를 취하지 않으면 빛의 방향과 normal벡터의 각이 180도가 넘어가는 즉 구 뒤쪽에 빛이 들어감
        diffseIntensity = saturate(dot(-viewLightDir, viewNormal));
        
    }
    // point
    else if (1 == lightInfo.type)
    {
        // 뷰 좌표계 상에서 광원의 위치를 알아낸다.
        // point light는 빛 object 주변에만 빛을 줘야하기 때문에 절대적인 direction으로 계산하지 않는다.
        // 표면까지의 방향을 따로 구해줘야한다.
        float3 lightViewPos = mul(float4(lightInfo.position.xyz, 1.f), view).xyz;
        
        // 광원의 위치에서 표면을 향하는 벡터
        viewLightDir = viewPos - lightViewPos;
        
        //광원에서 표면까지의 거리를 구한다.
        float dist = length(viewLightDir);
        
        // 광원에서 표면을 향하는 단위벡터를 구한다.
        viewLightDir = normalize(viewLightDir);
        
        // 빛의 감쇄
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535f * 0.5f);
        
        //view space 상에서 표면의 빛의 세기를 구함
        diffseIntensity = saturate(dot(-viewLightDir, viewNormal)) * ratio;
        
    }
    else
    {
        
    }
    // 반사광 세기를 구함
    // 표면의 빛의 반사벡터
    viewReflect = normalize(viewLightDir + 2.f * dot(-viewLightDir, viewNormal) * viewNormal);
        
    // 시점에서 표면을 향하는 벡터
    eye = normalize(viewPos);
    
     //시선 벡터랑 반사벡터를 내적해서 반사광의 세기를 구한다.
    specularIntensity = saturate(dot(-eye, viewReflect));
    specularIntensity = pow(specularIntensity, 30);
    
    // 최종 난반사광
    lightColor.diffuse += lightInfo.color.diffuse * diffseIntensity;
        
    // 정반사광
    lightColor.specular += lightInfo.color.specular * specularIntensity;
        
    // 주변광
    lightColor.ambient = lightInfo.color.ambient;
}

float3 CalculateLightPBR_Direct(float3 worldPos, float4 albedo, float3 worldNormal, float metallic, float roughness)
{
    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // 빛 각도

    // PBR     
    float3 N = normalize(worldNormal); 
    float3 V = normalize(cameraWorldPos.xyz - worldPos); 
    float3 R = reflect(-V, N);

    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo.xyz, metallic); // 금속성이 강할수록 albedo를 사용하고 아니면 0.04사용 (재질 값)

    float NDotV = saturate(dot(N, V)); // 노멀 to 눈 반사각 
    float NdotL = max(dot(N, L), 0.f); // 노말과 빛

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    

    //float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
    float3 ks = F;
    float3 kd = 1.0f - ks;
    kd *= 1.0f - metallic;

    float3 irradiance = irradianceMap.Sample(skyBoxSampler, N).rgb;    
    float3 diffuse = irradiance * albedo.xyz;

    const float MAX_REFLECTION_LOD = 4.f;
    float3 prefilteredColor = prefilteredMap.SampleLevel(skyBoxSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    
    // brdf 텍스처를 x는 반사에 따라 y는 러프니스에 따라 샘플링한다.
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.0), roughness)).rg;
    
                                        // 반사 관련된 x는 프레넬과 계산 y는 diffuse처럼 생각하는 듯 보임
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
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
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
        const float nearZ = 0.01; // 카메라 설정과 동일
        
        // 1. Project posWorld to light screen    
        float4 lightScreen = mul(float4(posWorld, 1.0), lightProjection);
        lightScreen.xyz /= lightScreen.w;
        
        // 2. 카메라(광원)에서 볼 때의 텍스춰 좌표 계산
        float2 lightTexcoord = float2(lightScreen.x, -lightScreen.y);
        lightTexcoord += 1.0;
        lightTexcoord *= 0.5;
        
        // 3. 쉐도우맵에서 값 가져오기
        float depth = shadowMap.Sample(linearSampler, lightTexcoord).r;
        //float depth = shadowMap.Sample(shadowPointSampler, lightTexcoord).r;
        
        // 4. 가려져 있다면 그림자로 표시
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
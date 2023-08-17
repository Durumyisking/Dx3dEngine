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

float3 CalculateLightPBR_Direct(float3 viewPos, float4 albedo, float3 viewNormal, float metallic, float roughness)
{
    // PBR     
    float3 V = normalize(-viewPos); // 뷰공간 pinPoint(0,0,0)부터 픽셀로 향하는 벡터
    float3 N = viewNormal; // 정점/텍스처 노말 뷰변환 완료
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // 노멀 to 눈 반사각 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // 금속성이 강할수록 albedo를 사용하고 아니면 0.04사용 (재질 값)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // 빛 각도            

    float NdotL = max(dot(N, L), 0.f);

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);

    float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);

    float3 irradiance = irradianceMap.Sample(linearSampler, N).rgb;
    float3 diffuse = irradiance * kd * albedo.xyz;
    //float3 diffuse = kd * albedo.xyz;

    const float MAX_REFLECTION_LOD = 4.f;
    float3 prefilteredColor = prefilteredMap.SampleLevel(linearSampler, R, roughness * MAX_REFLECTION_LOD).rgb; // 프레임을 많이 잡아먹는듯?
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.f), roughness)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y); 
    //float3 specular = (F * envBRDF.x + envBRDF.y);

    float3 result = (diffuse + specular) * NdotL;
    //float3 ambient = (diffuse + specular); // 마딧세이는 전방향 빛 비추는듯?
    
    return result;
}
float3 CalculateLightPBR_Diffuse(float3 viewPos, float4 albedo, float3 viewNormal, float metallic, float roughness)
{
    // PBR     
    float3 V = normalize(-viewPos);
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
    float3 V = normalize(-viewPos); // 뷰공간 pinPoint(0,0,0)부터 픽셀로 향하는 벡터
    float3 N = viewNormal; // 정점/텍스처 노말 뷰변환 완료
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // 노멀 to 눈 반사각 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // 금속성이 강할수록 albedo를 사용하고 아니면 0.04사용 (재질 값)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // 빛 각도            

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
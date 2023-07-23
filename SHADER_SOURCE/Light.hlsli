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

// microfacet 계산
float NormalDistributionGGXTR(float3 normal, float3 viewReflectDir, float roughness)
{
    float roughness2 = roughness * roughness;
    float NdotH = saturate(dot(normal, viewReflectDir)); // normal과 halfvec의 각도를 구한다.
    float NdotH2 = NdotH * NdotH;

    float nom = roughness2;
    float denom = (NdotH2 * (roughness2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

// 에너지 보존법칙 계산
// 거칠기에 따른 쉐도잉과 마스킹 연산
// 거칠기 up = 마스킹 쉐도잉 up

float GeometrySchlickGGX(float NdotV, float roughness)  
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f; 
    // k 는 ibl 조명을 이나 directional light에 따라 다른게 배치된다.

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

    float ggx1 = GeometrySchlickGGX(NdotV, roughness); // 시야 방향에 대한 그림자
    float ggx2 = GeometrySchlickGGX(NdotL, roughness); // 빛의 방향에 대한 그림자

    return ggx1 * ggx2;
}

// Fresnel 광택 계산 함수                             // eyedir
float3 FresnelSchlick(float3 albedo, float metallic, float3 viewDir, float3 viewReflectDir)
{
    // Fresnel 광택 계수
    float3 F0 = (float3) 0.4f; // 국룰인가봄
    
    F0 = lerp(F0, albedo, metallic);

    // Fresnel Schlick 근사
    viewDir = normalize(viewDir);
    viewReflectDir = normalize(viewReflectDir);
    
    float VoH = saturate(dot(viewDir, normalize(viewReflectDir))); // 빛 입사벡터와 표면 normal 사이의 각 1에 가까울수록 수직 입사
    float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - VoH, 5.0);

    return fresnel;
}



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
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535 * 0.5f);
        
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
    float cosLo = max(0.0, dot(viewNormal, eyeDir)); // 카메라 dir과 , 법선벡터와의 0 이상인 각도를 반환
		
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * viewNormal - eyeDir;

    // Directional
    if (0 == lightInfo.type)
    {
        // 광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환
        // direction은 lightObject의 transform의 forward 입니다.
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
        // 뷰 좌표계 상에서 광원의 위치를 알아낸다.
        // point light는 빛 object 주변에만 빛을 줘야하기 때문에 절대적인 direction으로 계산하지 않는다.
        // 표면까지의 방향을 따로 구해줘야한다.
        float3 lightViewPos = mul(float4(lightInfo.position.xyz, 1.f), view).xyz;
        
        // 광원의 위치에서 표면을 향하는 벡터
        viewLightDir = lightViewPos - viewPos;
        
        //광원에서 표면까지의 거리를 구한다.
        float dist = length(viewLightDir);
        
        // 광원에서 표면을 향하는 단위벡터를 구한다.
        viewLightDir = normalize(viewLightDir);
        
        float cosTheta = max(dot(viewNormal, viewLightDir), 0.0); // 각에 따른 빛의 세기
        
        // 빛의 감쇄
        float ratio = cos(saturate(dist / lightInfo.radius) * 3.1415926535 * 0.5f);    
                
        //view space 상에서 표면의 빛의 세기를 구함
        radiance = lightInfo.color.diffuse.xyz * ratio * cosTheta; // 최종 입사 빛
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
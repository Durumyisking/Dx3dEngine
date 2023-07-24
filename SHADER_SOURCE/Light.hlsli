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
        viewLightDir = normalize(mul(float4(lightInfo.direction.xyz, 0.f), view));

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
        float3 lightViewPos = mul(float4(lightInfo.position.xyz, 1.f), view);
        
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

//3D
static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);
#include "Random.hlsli"
#include "BRDF.hlsli"
#include "Texture.hlsli"
#include "Sampler.hlsli"

struct LightColor
{
    float4 diffuse;

    // pbr에서 아래 내용들은 쓰지 않도록 합시다
    float4 specular;
    float4 ambient;    
};

struct LightAttribute
{
    LightColor color;

    float4 position;
    float4 direction;
    
    // point and spot 
    float radius;
    float fallOffStart;
    float fallOffEnd;
    float spotPower;
    
    int type;      
    float3 padding;

    row_major matrix view;
    row_major matrix projection;
    row_major matrix inverseProjection;
};

//#define LIGHT_OFF 0x00
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2
//#define LIGHT_SHADOW3 0x10
#define NEAR_PLANE 1.f
// #define LIGHT_WORLD_RADIUS 0.001
#define LIGHT_FRUSTUM_WIDTH 0.61584 // <- 계산해서 찾은 값

StructuredBuffer<LightAttribute> lightAttributes : register(t22);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

static const float Epsilon = 0.00001f;
static const float3 Fdielectric = 0.04f;

        
static const float2 diskSamples64[64] =
{
    float2(0.0, 0.0),
            float2(-0.12499844227275288, 0.000624042775189866), float2(0.1297518688031755, -0.12006020382326336),
            float2(-0.017851253586055427, 0.21576916541852392), float2(-0.1530983013115895, -0.19763833164521946),
            float2(0.27547541035593626, 0.0473106572479027), float2(-0.257522587854559, 0.16562643733622642),
            float2(0.0842605283808073, -0.3198048832600703), float2(0.1645196099088727, 0.3129429627830483),
            float2(-0.3528833088400373, -0.12687935349026194), float2(0.36462214742013344, -0.1526456341030772),
            float2(-0.17384046457324884, 0.37637015407303087), float2(-0.1316547617859344, -0.4125130588224921),
            float2(0.3910687393754993, 0.2240317858770442), float2(-0.45629121277761536, 0.10270505898899496),
            float2(0.27645268679640483, -0.3974278701387824), float2(0.06673001731984558, 0.49552709793561556),
            float2(-0.39574431915605623, -0.33016879600548193), float2(0.5297612167716342, -0.024557141621887494),
            float2(-0.3842909284448636, 0.3862583103507092), float2(0.0230336562454131, -0.5585422550532486),
            float2(0.36920334463249477, 0.43796562686149154), float2(-0.5814490172413539, -0.07527974727019048),
            float2(0.4903718680780365, -0.3448339179919178), float2(-0.13142003698572613, 0.5981043168868373),
            float2(-0.31344141845114937, -0.540721256470773), float2(0.608184438565748, 0.19068741092811003),
            float2(-0.5882602609696388, 0.27536315179038107), float2(0.25230610046544444, -0.6114259003901626),
            float2(0.23098706800827415, 0.6322736546883326), float2(-0.6076303951666067, -0.31549215975943595),
            float2(0.6720886334230931, -0.1807536135834609), float2(-0.37945598830371974, 0.5966683776943834),
            float2(-0.1251555455510758, -0.7070792667147104), float2(0.5784815570900413, 0.44340623372555477),
            float2(-0.7366710399837763, 0.0647362251696953), float2(0.50655463562529, -0.553084443034271),
            float2(8.672987356252326e-05, 0.760345311340794), float2(-0.5205650355786364, -0.5681215043747359),
            float2(0.7776435491294021, 0.06815798190547596), float2(-0.6273416101921778, 0.48108471615868836),
            float2(0.1393236805531513, -0.7881712453757264), float2(0.4348773806743975, 0.6834703093608201),
            float2(-0.7916014213464706, -0.21270211499241704), float2(0.7357897682897174, -0.38224784745000717),
            float2(-0.2875567908732709, 0.7876776574352392), float2(-0.3235695699691864, -0.7836151691933712),
            float2(0.7762165924462436, 0.3631291803355136), float2(-0.8263007976064866, 0.2592816844184794),
            float2(0.4386452756167397, -0.7571098481588484), float2(0.18988542402304126, 0.8632459242554175),
            float2(-0.7303253445407815, -0.5133224046555819), float2(0.8939004035324556, -0.11593993515830946),
            float2(-0.5863762307291154, 0.6959079795748251), float2(-0.03805753378232556, -0.9177699189461416),
            float2(0.653979655650389, 0.657027860897389), float2(-0.9344208130797295, -0.04310155546401203),
            float2(0.7245109901504777, -0.6047386420191574), float2(-0.12683493131695708, 0.9434844461875473),
            float2(-0.5484582700240663, -0.7880790100251422), float2(0.9446610338564589, 0.2124041692463835),
            float2(-0.8470120123194587, 0.48548496473788055), float2(0.29904134279525085, -0.9377229203230629),
            float2(0.41623562331748715, 0.9006236205438447),
};

static const float2 poissonDisk[16] =
{
    float2(-0.94201624, -0.39906216), float2(0.94558609, -0.76890725),
            float2(-0.094184101, -0.92938870), float2(0.34495938, 0.29387760),
            float2(-0.91588581, 0.45771432), float2(-0.81544232, -0.87912464),
            float2(-0.38277543, 0.27676845), float2(0.97484398, 0.75648379),
            float2(0.44323325, -0.97511554), float2(0.53742981, -0.47373420),
            float2(-0.26496911, -0.41893023), float2(0.79197514, 0.19090188),
            float2(-0.24188840, 0.99706507), float2(-0.81409955, 0.91437590),
            float2(0.19984126, 0.78641367), float2(0.14383161, -0.14100790)
};

static const float2 mySamples9[9] =
{
    float2(-1.f, -1.f), float2(0.f, -1.f), float2(1.f, -1.f),
    float2(-1.f, 0.f), float2(0.f, 0.f), float2(1.f, 0.f),
    float2(-1.f, 1.f), float2(0.f, 1.f), float2(1.f, 1.f),
};


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

    const float MAX_REFLECTION_LOD = 6.f;
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

    //color.rgb += lightColor.specular.rgb;
    
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
                                                            0 + roughness * 5.f).rgb;
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


float VSM_FILTER(float2 moments, float fragDepth)
{
    float lit = (float) 1.0f;
    float E_x2 = moments.y;
    float Ex_2 = moments.x * moments.x;
    float variance = E_x2 - Ex_2;
    variance = max(variance, 0.00005f);

    float mD = moments.x - fragDepth; // mean dist
    float mD_2 = mD * mD;
    float p = variance / (variance + mD_2);

    lit = max(p, fragDepth <= moments.x);
    
    return lit;
}
// NdcDepthToViewDepth
float N2V(float ndcDepth, matrix invProj)
{
    float4 pointView = mul(float4(0, 0, ndcDepth, 1), invProj);
    return pointView.z / pointView.w;
}



// Assuming that LIGHT_FRUSTUM_WIDTH == LIGHT_FRUSTUM_HEIGHT
// #define LIGHT_RADIUS_UV (LIGHT_WORLD_RADIUS / LIGHT_FRUSTUM_WIDTH)

float PCF_Filter(float2 uv, float zReceiverNdc, float filterRadiusUV, Texture2D shadowMap)
{
    float sum = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        float2 offset = diskSamples64[i] * filterRadiusUV;
        sum += shadowMap.SampleCmpLevelZero(
            shadowCompareSampler, uv + offset, zReceiverNdc);
    }
    return sum / 64;
}

// void Func(out float a) <- c++의 void Func(float& a) 처럼 출력값 저장 가능

void FindBlocker(out float avgBlockerDepthView, out float numBlockers, float2 uv,
                 float zReceiverView, Texture2D shadowMap, matrix invProj, float lightRadiusWorld)
{
    float lightRadiusUV = lightRadiusWorld / LIGHT_FRUSTUM_WIDTH;
    
    float searchRadius = lightRadiusUV * (zReceiverView - NEAR_PLANE) / zReceiverView;

    float blockerSum = 0;
    numBlockers = 0;
    for (int i = 0; i < 64; ++i)
    {
        float shadowMapDepth =
            shadowMap.SampleLevel(shadowPointSampler, float2(uv + diskSamples64[i] * searchRadius), 0).r;

        shadowMapDepth = N2V(shadowMapDepth, invProj);
        
        if (shadowMapDepth < zReceiverView)
        {
            blockerSum += shadowMapDepth;
            numBlockers++;
        }
    }
    avgBlockerDepthView = blockerSum / numBlockers;
}
float PCSS(float2 uv, float zReceiverNdc, Texture2D shadowMap, matrix invProj, float lightRadiusWorld)
{
    float lightRadiusUV = lightRadiusWorld / LIGHT_FRUSTUM_WIDTH;
    
    float zReceiverView = N2V(zReceiverNdc, invProj);
    
    // STEP 1: blocker search
    float avgBlockerDepthView = 0;
    float numBlockers = 0;

    FindBlocker(avgBlockerDepthView, numBlockers, uv, zReceiverView, shadowMap, invProj, lightRadiusWorld);

    if (numBlockers < 1)
    {
        // There are no occluders so early out(this saves filtering)
        return 1.0f;
    }
    else
    {
        // STEP 2: penumbra size
        float penumbraRatio = (zReceiverView - avgBlockerDepthView) / avgBlockerDepthView;
        float filterRadiusUV = penumbraRatio * lightRadiusUV * NEAR_PLANE / zReceiverView;

        // STEP 3: filtering
        return PCF_Filter(uv, zReceiverNdc, filterRadiusUV, shadowMap);
    }
}

float3 LightRadiance(in LightAttribute light, in float3 posWorld, in float3 normalWorld)
{
    // Directional light
    float3 lightVec = light.type & LIGHT_DIRECTIONAL
                      ? -light.direction.xyz
                      : light.position.xyz - posWorld;
        
    float lightDist = length(lightVec);
    lightVec /= lightDist;

    // Spot light
    float spotFator = light.type & LIGHT_SPOT
                     ? pow(max(-dot(lightVec, light.direction.xyz), 0.0f), light.spotPower)
                      : 1.0f;
        
    // Distance attenuation
    //float att = saturate((light.fallOffEnd - lightDist)
    //                     / (light.fallOffEnd - light.fallOffStart));

    float att = light.type == LIGHT_DIRECTIONAL
        ? 1.0f
        : saturate((light.fallOffEnd - lightDist) / (light.fallOffEnd - light.fallOffStart));

    
    // Shadow map
    float shadowFactor = 1.0;
    float3 radiance = light.color.diffuse.xyz * spotFator * att * shadowFactor;

    return radiance;
}
float3 LightRadiance(LightAttribute light, float3 posWorld, float3 normalWorld, Texture2D shadowMap)
{
     // Directional light
    float3 lightVec = light.type & LIGHT_DIRECTIONAL
                      ? -light.direction.xyz
                      : light.position.xyz - posWorld;
        
    float lightDist = length(lightVec);
    lightVec /= lightDist;

    // Spot light
    float spotFator = light.type & LIGHT_SPOT
                     ? pow(max(-dot(lightVec, light.direction.xyz), 0.0f), light.spotPower)
                      : 1.0f;
        
     // Distance attenuation
    //float att = saturate((light.fallOffEnd - lightDist)
    //                     / (light.fallOffEnd - light.fallOffStart));

    float att = light.type == LIGHT_DIRECTIONAL
        ? 1.0f
        : saturate((light.fallOffEnd - lightDist) / (light.fallOffEnd - light.fallOffStart));

    // Shadow map
    float shadowFactor = 1.0;

    { 
        // 1. Project posWorld to light screen    
        float4 lightScreen = mul(float4(posWorld, 1.0), light.view);
        lightScreen = mul(float4(lightScreen.xyz, 1.0), light.projection);
        lightScreen.xyz /= lightScreen.w;
        
        // 2. 카메라(광원)에서 볼 때의 텍스춰 좌표 계산
        float2 lightTexcoord =
        float2(
            (lightScreen.x * 0.5) + 0.5f,
            -(lightScreen.y * 0.5) + 0.5f
        );
        
        // Baisc Shadow
        /*
            // 3. 쉐도우맵에서 값 가져오기
            float depth = shadowMap.Sample(clampSampler, lightTexcoord).r;
        
            // 4. 가려져 있다면 그림자로 표시
            if (depth + 0.00001 < lightScreen.z)
                shadowFactor = 0.0;
        
            
            ///////////////////////////////////////////////////////////////
            dx11에서는 위 코드를 함수로 사용할 수 있게 만들어놓음
            shadowMap.SampleCmpLevelZero(
            shadowCompareSampler, lightTexcoord.xy, lightScreen.z - 0.00001);
        
            SampleCmpLevelZero : SampleCompare의 Mip 수준을 0으로 사용하는 것
            SampleCmp          : 텍스처에서 샘플링 해 오는 값들 중 r 값만 3번째 인자인 compare value와 비교하는 것 (depth texture을 샘플링해서 값을 가져오고 그 값이 compare value보다 작으면 0 else = 1로 처리)
                                 샘플링 하는 픽셀 주변 4개값의 평균값을 구해서 샘플링한다. 즉 (0~1의 값이 나온다)
        */
        
        
        // pcf sampliing
        
        uint width, height, numMips;
        shadowMap.GetDimensions(0, width, height, numMips);

        // Texel size
        float dx = 5.0 / (float) width;
        shadowFactor = PCF_Filter(lightTexcoord.xy, lightScreen.z - 0.00001, dx, shadowMap);
        
        
        // pcss sampling
        //shadowFactor = PCSS(lightTexcoord, lightScreen.z - 0.01, shadowMap, light.inverseProjection, 10.f);
        
        // vsm sampling
        //shadowFactor = VSM_FILTER(ShadowMap.Sample(linearSampler, lightTexcoord).rg, lightScreen.z);
        
    }

    float3 radiance = light.color.diffuse.xyz * spotFator * att * shadowFactor;

    return radiance;
}



float3 PBR_DirectLighting(float3 pixelToEye, float3 lightDir, float3 albedo, float3 normal, float metallic, float roughness)
{
    float3 directLighting = (float3) 0.f;   
    
    // dir light빛 방향 월드 기준일거임
    float3 lightVec = -normalize(float4(lightAttributes[0].direction.xyz, 0.f)).xyz;

    float3 halfway = normalize(pixelToEye + lightVec);
        
    float NdotI = max(0.0, dot(normal.xyz, lightVec));
    float NdotH = max(0.0, dot(normal.xyz, halfway));
    float NdotO = max(0.0, dot(normal.xyz, pixelToEye));
        
    const float3 Fdielectric = 0.4f; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic);
    float3 F = fresnelSchlick(F0, max(0.0, dot(halfway, pixelToEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo.xyz;

    float D = ndfGGX(NdotH, roughness);
    float3 G = gaSchlickGGX(NdotI, NdotO, roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

    // float3 radiance = lightAttributes[0].color.diffuse.xyz;
      
    directLighting += (diffuseBRDF + specularBRDF); // * NdotI;
    
    return directLighting;
}


//3D
static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);
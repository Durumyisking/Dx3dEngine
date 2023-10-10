#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    
    uint Instance : SV_InstanceID; // drawindexedinstance 하면 자동으로 들어간다.
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    float3 ViewPos : POSITION;

    float3 ViewTangent : TANGENT;
    float3 ViewNormal : NORMAL;
    
    uint Instance : SV_InstanceID;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    float4x4 particleWorld = particleBuffer[vsIn.Instance].particleWorld;
    
    float4 worldPos = mul(vsIn.Position, particleWorld);
    float4 veiwPos = mul(worldPos, view);
    float4 projectionPos = mul(veiwPos, projection);
    
    
    // 노말 탄젠트 계산
    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.f), transpose(particleWorld)).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.f), view).xyz);
    
    float3 viewTangent = normalize(mul(float4(vsIn.Tangent.xyz, 0.f), particleWorld).xyz);
    viewTangent = normalize(mul(float4(viewTangent, 0.f), view).xyz);
    
    
    // VSOUT 초기화
    vsOut.Position = projectionPos;
    vsOut.UV = vsIn.UV;
    
    vsOut.ViewPos = worldPos.xyz;
    
    vsOut.ViewNormal = viewNormal;
    vsOut.ViewTangent = viewTangent;
    
    
    vsOut.Instance = vsIn.Instance;
    
    return vsOut;
}


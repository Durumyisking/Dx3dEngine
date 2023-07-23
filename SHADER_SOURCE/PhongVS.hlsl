#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    
    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;

    float Intensity : FOG;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    
    float4 skeltonPostion = mul(vsIn.Position, cbmat4);
    float4 worldPosition = mul(skeltonPostion, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    vsOut.Position = projPosition;
    vsOut.UV = vsIn.UV;

    // 로컬 노말을 뷰변환
    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.0f), world).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.0f), view).xyz);
    
    vsOut.ViewPos = viewPosition.xyz;
    vsOut.ViewNormal = viewNormal.xyz;
    
    return vsOut;
}

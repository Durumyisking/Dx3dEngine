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
    // 해당 옵션을 붙이면 레스터라이저에서 보간을 수행하지 않습니다
    nointerpolation float3 ViewNormal : NORMAL;

    float intensity : FOG;
};

VSOut main(VSIn vsIn)
{
    VSOut OUT = (VSOut) 0.0f;
    
    float4 worldPosition = mul(vsIn.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
    
    OUT.Position = projPosition;
    OUT.UV = vsIn.UV;
    
    // 로컬 노말을 뷰변환
    float3 vViewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.0f), world).xyz);
    vViewNormal = normalize(mul(float4(vViewNormal, 0.0f), view).xyz);
    
    OUT.ViewPos = viewPosition.xyz;
    OUT.ViewNormal = vViewNormal.xyz;
    
    return OUT;
}
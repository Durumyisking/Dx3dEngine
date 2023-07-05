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

    float intensity : FOG;
};

VSOut main(VSIn In)
{
    VSOut OUT = (VSOut) 0.0f;
    
    float4 worldPosition = mul(In.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    OUT.Position = ProjPosition;
    OUT.UV = In.UV;

    // 로컬 노말을 뷰변환
    float3 vViewNormal = normalize(mul(float4(In.Normal.xyz, 0.0f), world).xyz); 
    vViewNormal = normalize(mul(float4(vViewNormal, 0.0f), view).xyz);
    
    OUT.ViewPos = viewPosition.xyz;
    OUT.ViewNormal = vViewNormal.xyz;
    
    return OUT;
}
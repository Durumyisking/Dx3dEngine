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
    float3 Normal : NORMAL; 
};

VSOut main(VSIn In)
{
    VSOut OUT = (VSOut) 0.0f;
    
    float4 worldPosition = mul(In.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    OUT.Position = ProjPosition;
    OUT.UV = In.UV;
    
    OUT.Normal = mul(float4(In.Normal.xyz, 0.0f), world).xyz; // Flat shading을 위해 월드 좌표계 법선 벡터 저장
    
    return OUT;
}
#include "global.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn In)
{
    VSOut ouput = (VSOut) 0.0f;
    
    //float4 worldPosition = mul(float4(In.Pos, 1.0f), world);
    //float4 viewPosition = mul(worldPosition, view);
    //float4 ProjPosition = mul(viewPosition, projection);
    In.Pos.y += 0.49f;
    float4 viewPos = mul(float4(In.Pos, 0.0f), view);
    float4 projPos = mul(float4(viewPos.xyz, 1.0f), fovForSkySphere);
    projPos.z = projPos.w;
    ouput.Pos = projPos;
    ouput.UV = In.UV;
    
    return ouput;
}
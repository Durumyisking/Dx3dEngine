#include "global.hlsli"

struct VSIn
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn vsIn)
{
    VSOut ouput = (VSOut) 0.0f;
    
    //float4 worldPosition = mul(float4(In.Pos, 1.0f), world);
    //float4 viewPosition = mul(worldPosition, view);
    //float4 ProjPosition = mul(viewPosition, projection);
    vsIn.Position.y += 0.49f;
    float4 viewPos = mul(float4(vsIn.Position, 0.0f), view);
    float4 projPos = mul(float4(viewPos.xyz, 1.0f), fovForSkySphere);
    projPos.z = projPos.w;
    ouput.Position = projPos;
    ouput.UV = vsIn.UV;
    
    return ouput;
}
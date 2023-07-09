#include "global.hlsli"

struct VS_IN
{
    float4 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT main(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f; // √ ±‚»≠
  
    float4 worldPosition = mul(_in.vPos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projectionPosition = mul(viewPosition, projection);
    
    output.vPos = projectionPosition;
    output.vUV = _in.vUV;
    
    return output;
}
#include "global.hlsli"


VTX_OUT main(VTX_IN vtxIn)
{
    VTX_OUT output = (VTX_OUT) 0.f; 
    
    float4 worldPosition = mul(vtxIn.vPos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projectionPosition = mul(viewPosition, projection);
    
    output.vPos = projectionPosition;
    output.vUV = vtxIn.vUV;
       
    return output;
}
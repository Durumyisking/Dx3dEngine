#include "global.hlsli"


VTX_OUT main(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
  
    float4 worldPosition = mul(vtxIn.vPos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projectionPosition = mul(viewPosition, projection);
    
    output.vPos = projectionPosition;
    output.vColor = vtxIn.vColor;
    output.vUV = vtxIn.vUV;

    return output; // �긦 �����Ͷ������� ������
}
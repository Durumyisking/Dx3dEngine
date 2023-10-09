#include "global.hlsli"

float4 main(VTX_OUT vsIn) : SV_Target
{
    float4 color = RED;
    
    color = colorTexture.Sample(linearSampler, vsIn.vUV);
    
    color *= cbxyzw1; // ���� ��
    color += cbxyzw2; // ���� ��
    
    if (color.w == 0)
        discard;    
    
    return color;
}
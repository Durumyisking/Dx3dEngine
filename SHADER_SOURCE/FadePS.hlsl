#include "global.hlsli"



float4 main(VTX_OUT vtxIn) : SV_TARGET
{   
    float4 color = { 0.f, 0.f, 0.f, 1.f };
   
    // 1�̸� �ؽ�ó�� fadeinout�Ұ���
    if (cbiData2 == 1)
        color = colorTexture.Sample(pointSampler, vtxIn.vUV);
    
//    float alpha = color.w;
    float alpha;
    
    if (cbiData1 == 0) // 0 : fade out, 1 : in
    {
        alpha = saturate(1 - (cbfData1));
    }
    else if (cbiData1 == 1)
    {
        alpha = saturate(0 + (cbfData1));

    }
//    color.w = alpha;
    
    return color * alpha;
}
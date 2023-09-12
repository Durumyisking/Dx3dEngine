#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
};

struct PSOut
{
    float4 depthColor : SV_Target0;

};

PSOut main(VSOut vsIn) : SV_Target
{
    PSOut psOut;
    
    float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // �ȼ����̴��� �Ѿ���� ������ ����(z)�� ���ٳ����� ó�� ������ �ʰ� �Ѿ�´�.
    // ���� ���� �������
    float depth = vsIn.ProjPosition.z / vsIn.ProjPosition.w; // z���� Ŭ���� �� ū ���� ���´�
    //output = (depth.xxx, 1.0f);
    psOut.depthColor.r = depth;
    psOut.depthColor.g = depth * depth;
    psOut.depthColor.w = 1.0f;
    //output.rgb = In.ProjPosition.z / In.ProjPosition.w;
    //output.rgb *= (900 / 1600);
    
    return psOut;
}
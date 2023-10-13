#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
    float4 WVP : POSITION1;
};

struct PSOut
{
    float4 depthColor : SV_Target0;
    float4 depthMap : SV_Target1;
};

PSOut main(VSOut vsIn) : SV_TARGET
{
    PSOut psOut = (PSOut) 0.f;
    
    float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // �ȼ����̴��� �Ѿ���� ������ ����(z)�� ���ٳ����� ó�� ������ �ʰ� �Ѿ�´�.
    // ���� ���� �������
    float depth = vsIn.ProjPosition.z / vsIn.ProjPosition.w; // z���� Ŭ���� �� ū ���� ���´�
    //output = (depth.xxx, 1.0f);
    psOut.depthColor.r = depth;
    // decal
    psOut.depthMap.r = vsIn.WVP.z / 1000.f;
    
    // vsm ���� Ǯ��
    //psOut.depthColor.g = depth * depth;
    //output.rgb = In.ProjPosition.z / In.ProjPosition.w;
    //output.rgb *= (900 / 1600);
    
    return psOut;
}
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
    
    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;
    float intensity : FOG;
};

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    LightAttribute lightAttribute = lightAttributes[0];
       
    //������ ������ ���� ��ǥ�迡�� �� ��ǥ��� ��ȯ
    float3 viewLightDir = normalize(mul(float4(lightAttribute.direction.xyz, 0.0f), view));
    
    // ���⿡ ������ ���ϴ� ����
    // ���� ǥ�鿡 ǥ���ϰ� �ͱ⶧�� (������ ������ �������� 1�� �������)
    // ������ ������ ������ ���� ����� normal������ ���� 180���� �Ѿ�� �� �� ���ʿ� ���� ��
    float intensity = saturate(dot(-viewLightDir, vsIn.ViewNormal));
    float fSpecPow = 0.0f;
    
    //�� �����̽� �󿡼� ǥ���� ���� ���⸦ ���ؾ���
    float3 vViewReflect
    = normalize(viewLightDir + 2.0f * dot(-viewLightDir, vsIn.ViewNormal) * vsIn.ViewNormal);
    
    // �������� ǥ���� ���ϴ� ����
    float3 vEye = normalize(vsIn.ViewPos);
    
    //�ü� ���Ͷ� �ݻ纤�͸� �����ؼ� �ݻ籤�� ���⸦ ���Ѵ�.
    fSpecPow = saturate(dot(-vEye, vViewReflect));
    
    // fSpectPow�� 30��
    fSpecPow = pow(fSpecPow, 30);
    
    // �л걤
    outColor.rgb *= lightAttribute.color.diffuse.rgb * intensity;

    // �ݻ籤
    outColor.rgb += lightAttribute.color.specular.rgb * fSpecPow * 0.5f;

    // �ֺ���
    outColor.rgb += (outColor.xyz * lightAttribute.color.ambient.rgb);
    
    //float4 Out = float4(1.0f, 0.0f, 1.0f, 1.0f);
    //float3 LightDir = normalize(globalLightDir);
    //float intensity = saturate(dot(-LightDir, In.Normal));
    
    //Out.xyz = (intensity * OutColor.xyz * globalLightColor)
    //            + (OutColor.xyz * globalLightAmb);
    
    return outColor;
        
}
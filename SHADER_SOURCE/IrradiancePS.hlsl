#include "global.hlsli"


struct VSOut
{
    float4 Position : SV_Position;
    float3 LocalPos : Position;
};

float4 main(VSOut psIn) : SV_TARGET
{
    float3 normal = normalize(psIn.LocalPos);
    float3 irradiance = float3(0.0, 0.0, 0.0);

    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.012;
    float nrSamples = 0.0;

    float PI_2 = 2.0 * PI;
    float PI_05 = 0.5 * PI;
    
    // �ݱ� ������� �ݺ�
    for (float phi = 0.0; phi < PI_2; phi += sampleDelta) // ����󿡼��� �� �� ����
    {
        for (float theta = 0.0; theta < PI_05; theta += sampleDelta) // ����󿡼��� �� �Ʒ� ����
        {
			// ���� -> ��ī��Ʈ
            float3 cartesianSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// ��ī��Ʈ -> ����
            float3 sampleVec = cartesianSample.x * right + cartesianSample.y * up + cartesianSample.z * normal;
            float weight = cos(theta) * sin(theta);
            irradiance += CubeMapTexture.Sample(linearSampler, -sampleVec).rgb * weight;
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.0);
}
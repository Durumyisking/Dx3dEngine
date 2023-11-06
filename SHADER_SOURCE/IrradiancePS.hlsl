#include "global.hlsli"


struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

float4 main(VSOut psIn) : SV_TARGET
{
    float3 normal = normalize(psIn.WorldPos);
    float3 irradiance = float3(0.0, 0.0, 0.0);

    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.012;
    float nrSamples = 0.0;

    
    for (float phi = 0.0; phi < 2.0 * 3.1415926535f; phi += sampleDelta) // 구면상에서의 좌 우 각도
    {
        for (float theta = 0.0; theta < 0.5 * 3.1415926535f; theta += sampleDelta) // 구면상에서의 위 아래 각도
        {
			// 구면 -> 데카르트
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// 데카르트 -> world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += CubeMapTexture.Sample(linearSampler, -sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = 3.1415926535f * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.0);
}
#include "global.hlsli"


struct VSOut
{
    float4 Position : SV_Position;
    float3 WorldPos : Position;
};

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// 픽셀들을 샘플링할패턴을 구한다.
float2 Hammersley(uint i, uint N)
{
    // 0~1
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

//GGX(Geometry, Grin, and X) 분포를 사용하여 샘플링을 수행하는 함수
// 거칠기를 통한 미세 표면 half벡터 생성
float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * 3.1415926535f * Xi.x;
    //float phi = 2.0 * 3.1415926535f * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// from spherical coordinates to cartesian coordinates
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

	// from tangent-space vector to world-space sample vector
    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);

    float3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float4 main(VSOut psIn) : SV_TARGET
{
    float roughness = 0.1f;
    
    // input으로 부터 현재 픽셀의 표면 법선을 계산0
    float3 N = normalize(psIn.WorldPos);
    float3 R = N;
    //뷰 방향을 반사 방향으로 초기화한다.
    float3 V = R;

    //const uint SAMPLE_COUNT = 1024u;
    //float totalWeight = 0.0;
    float3 prefilteredColor = float3(0.0, 0.0, 0.0);
    //for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    //{
    //    // half verter 계산
    //    float2 Xi = Hammersley(i, SAMPLE_COUNT); // 샘플링 패턴
    //    float3 H = ImportanceSampleGGX(Xi, N, roughness);

    //    float3 L = normalize(2.0 * dot(V, H) * H - V);

    //    float NdotL = max(dot(N, L), 0.0);
    //    if (NdotL > 0.0)
    //    {
    //        prefilteredColor += CubeMapTexture.Sample(linearSampler, -L).rgb * NdotL;
    //        totalWeight += NdotL;
    //    }
    //}
    //prefilteredColor = prefilteredColor / totalWeight;
    prefilteredColor += CubeMapTexture.Sample(linearSampler, -N).rgb;
    return float4(prefilteredColor, 1.0);

	//return tex.Sample(splr, input.worldPos);
	//return float4(color, 1.0f);
}
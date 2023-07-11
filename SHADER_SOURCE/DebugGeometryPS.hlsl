
struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(VS_OUT _in) : SV_Target
{
    return float4(0.f, 1.f, 0.f, 1.f);
}
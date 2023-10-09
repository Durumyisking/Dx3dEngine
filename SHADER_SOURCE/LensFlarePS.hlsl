#include "global.hlsli"

float2 tResolution = float2(234.f, 119.f);

float noise(float t)
{
    return NoiseTexture.Sample(linearSampler, float2(t, 0.0) / tResolution).x;
}
float noise(float2 t)
{
    return NoiseTexture.Sample(linearSampler, t / tResolution).x;
}

float3 lensflare(float2 uv, float2 pos)
{
    float2 main = uv - pos;
    float2 uvd = uv * (length(uv));
	
    float ang = atan2(main.x, main.y);
    float dist = length(main);
    dist = pow(dist, .1);
    float n = noise(float2(ang * 16.0, dist * 32.0));
	
    float f0 = 1.0 / (length(uv - pos) * 16.0 + 1.0);
	
    f0 = f0 + f0 * (sin(noise(sin(ang * 2. + pos.x) * 4.0 - cos(ang * 3. + pos.y)) * 16.) * .1 + dist * .1 + 0.8);
	
    float f1 = max(0.01 - pow(length(uv + 1.2 * pos), 1.9), .0) * 7.0;

    float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.8 * pos), 2.0)), .0) * 0.25;
    float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.85 * pos), 2.0)), .0) * 0.23;
    float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.9 * pos), 2.0)), .0) * 0.21;
	
    float2 uvx = lerp(uv, uvd, -0.5);
	
    float f4 = max(0.01 - pow(length(uvx + 0.4 * pos), 2.4), .0) * 6.0;
    float f42 = max(0.01 - pow(length(uvx + 0.45 * pos), 2.4), .0) * 5.0;
    float f43 = max(0.01 - pow(length(uvx + 0.5 * pos), 2.4), .0) * 3.0;
	
    uvx = lerp(uv, uvd, -.4);
	
    float f5 = max(0.01 - pow(length(uvx + 0.2 * pos), 5.5), .0) * 2.0;
    float f52 = max(0.01 - pow(length(uvx + 0.4 * pos), 5.5), .0) * 2.0;
    float f53 = max(0.01 - pow(length(uvx + 0.6 * pos), 5.5), .0) * 2.0;
	
    uvx = lerp(uv, uvd, -0.5);
	
    float f6 = max(0.01 - pow(length(uvx - 0.3 * pos), 1.6), .0) * 6.0;
    float f62 = max(0.01 - pow(length(uvx - 0.325 * pos), 1.6), .0) * 3.0;
    float f63 = max(0.01 - pow(length(uvx - 0.35 * pos), 1.6), .0) * 5.0;
	
    float3 c = (float3) 0.0;
	
    c.r += f2 + f4 + f5 + f6;
    c.g += f22 + f42 + f52 + f62;
    c.b += f23 + f43 + f53 + f63;
    c = c * 1.3 - (float3) length(uvd) * 0.05;
    c += (float3) f0;
	
    return c;
}

float3 cc(float3 color, float factor, float factor2) // color modifier
{
    float w = color.x + color.y + color.z;
    return lerp(color, (float3) (w) * factor, w * factor2);
}

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VSOut vsIn) : SV_TARGET
{
    float aspect = RESOLUTION.x / RESOLUTION.y;
    float2 uv = float2(vsIn.UV.x,  vsIn.UV.y);
    float2 p = -0.5 + uv;
    p.x *= aspect; // y = 1 x좌표는 비율로
    p.y *= -1.f; 
    
    //float3 dirLightPosition = float3(lightAttributes[0].position.xy / RESOLUTION.xy - 0.5, lightAttributes[0].position.z - 0.5);
    //dirLightPosition.x *= RESOLUTION.x / RESOLUTION.y; //fix aspect ratio
    
    float3 dirLightPosition = lightAttributes[0].position.xyz;
    dirLightPosition = mul(float4(dirLightPosition, 1.f), view);
    dirLightPosition = mul(float4(dirLightPosition, 1.f), projection);
    
    float3 color = postProcessTexture.Sample(linearSampler, uv);
    
    color += float3(1.4, 1.2, 1.0) * lensflare(p, dirLightPosition.xy);
    color -= noise(vsIn.UV.xy) * 0.015;
    color = cc(color, 0.5, 0.1);
    return float4(color, 1.0);
}
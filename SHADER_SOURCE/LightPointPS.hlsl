#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
};


struct PS_OUT
{
    float4 vDiffuse : SV_Target;
    float4 vSpecular : SV_Target1;
};

		//albedo = Resources::Find<Texture>(L"PositionTarget");
		//lightMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);
		//albedo = Resources::Find<Texture>(L"NormalTarget");
		//lightMaterial->SetTexture(eTextureSlot::NormalTarget, albedo);
		//albedo = Resources::Find<Texture>(L"SpecularTarget");
		//lightMaterial->SetTexture(eTextureSlot::SpecularTarget, albedo);

PS_OUT main(VSOut vsin)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = vsin.Position.xy / float2(1600.0f, 900.0f);
    float4 vViewPos = positionTarget.Sample(linearSampler, vUV);
    
    if (0.f == vViewPos.a)
        discard;
      
    // 광원 영역에 잡힌 position target의 위차값을 로컬영역으로 바꿔야한다.
    // 로컬 영역에서 광원메쉬 (spherer)의 내부에 있다면 실제로 point light 안에 들어가있다는 뜻
    float4 vLocalPos = mul(mul(vViewPos, inverseView), inverseWorld);
    if (length(vLocalPos.xyz) > 0.5f)
    {
        discard;
    }
    
    float4 vViewNormal = normalTarget.Sample(linearSampler, vUV);
        
    LightColor lightcolor = (LightColor) 0.f;
    CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, lightIndex, lightcolor);
    
    output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
    output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
       
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
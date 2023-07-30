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
    float4 vViewPos = positionTarget.Sample(anisotropicSampler, vUV);
    
    if (0.f == vViewPos.a)
        discard;
      
    // ���� ������ ���� position target�� �������� ���ÿ������� �ٲ���Ѵ�.
    // ���� �������� �����޽� (spherer)�� ���ο� �ִٸ� ������ point light �ȿ� ���ִٴ� ��
    float4 vLocalPos = mul(mul(vViewPos, inverseView), inverseWorld);
    if (length(vLocalPos.xyz) > 0.5f)
    {
        discard;
    }
    
    float4 vViewNormal = normalTarget.Sample(anisotropicSampler, vUV);
        
    LightColor lightcolor = (LightColor) 0.f;
    CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, lightIndex, lightcolor);
    
    float SpecCoef = specularTarget.Sample(anisotropicSampler, vUV).x;
    float4 vSpec = DecodeColor(SpecCoef);

    output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
    output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
       
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}
   
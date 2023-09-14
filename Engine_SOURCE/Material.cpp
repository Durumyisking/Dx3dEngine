#include "Material.h"
#include "GameObj.h"


Material::Material()
	:Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);
}
Material::Material(std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);
}
Material::Material(std::wstring textureColor, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);
}
Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);
}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);

}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::Roughness)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}

	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);

}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::Roughness)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
	mTexture[static_cast<UINT>(eTextureSlot::Emissive)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureEmissive);

	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}

	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);

}



Material::Material(std::wstring textureName, eTextureSlot slot, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{
	mTexture[static_cast<UINT>(slot)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureName);
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

	if (L"DeferredShader" == shaderName)
	{
		SetRenderingMode(eRenderingMode::DeferredOpaque);
	}
	mMaterialConstantBuffer.FresnelCoeff = Vector3(0.04f, 0.04f, 0.04f);

}

Material::~Material()
{
}
HRESULT Material::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}
void Material::SetData(eGPUParam param, void* data)
{
	switch (param)
	{
	case eGPUParam::Int_1:
		mMaterialConstantBuffer.iData1 = *static_cast<int*>(data);
		break;
	case eGPUParam::Int_2:
		mMaterialConstantBuffer.iData2 = *static_cast<int*>(data);
		break;
	case eGPUParam::Int_3:
		mMaterialConstantBuffer.iData3 = *static_cast<int*>(data);
		break;
	case eGPUParam::Int_4:
		mMaterialConstantBuffer.iData4 = *static_cast<int*>(data);
		break;
	case eGPUParam::Float_1:
		mMaterialConstantBuffer.fData1 = *static_cast<float*>(data);
		break;
	case eGPUParam::Float_2:
		mMaterialConstantBuffer.fData2 = *static_cast<float*>(data);
		break;
	case eGPUParam::Float_3:
		mMaterialConstantBuffer.fData3 = *static_cast<float*>(data);
		break;
	case eGPUParam::Float_4:
		mMaterialConstantBuffer.fData4 = *static_cast<float*>(data);
		break;
	case eGPUParam::Float_5:
		mMaterialConstantBuffer.fData5 = *static_cast<float*>(data);
		break;
	case eGPUParam::Float_6:
		mMaterialConstantBuffer.fData6 = *static_cast<float*>(data);
		break;
	case eGPUParam::Metallic:
		mMaterialConstantBuffer.metallic = *static_cast<float*>(data);
		break;
	case eGPUParam::Roughness:
		mMaterialConstantBuffer.roughness = *static_cast<float*>(data);
		break;
	case eGPUParam::Vector2_1:
		mMaterialConstantBuffer.xy1 = *static_cast<Vector2*>(data);
		break;
	case eGPUParam::Vector2_2:
		mMaterialConstantBuffer.xy2 = *static_cast<Vector2*>(data);
		break;
	case eGPUParam::Vector2_3:
		mMaterialConstantBuffer.xy3 = *static_cast<Vector2*>(data);
		break;
	case eGPUParam::Vector2_4:
		mMaterialConstantBuffer.xy4 = *static_cast<Vector2*>(data);
		break;
	case eGPUParam::Vector3_1:
		mMaterialConstantBuffer.xyz1 = *static_cast<Vector3*>(data);
		break;
	case eGPUParam::Vector3_2:
		mMaterialConstantBuffer.xyz2 = *static_cast<Vector3*>(data);
		break;
	case eGPUParam::FresnelCoeff:
		mMaterialConstantBuffer.FresnelCoeff = *static_cast<Vector3*>(data);
		break;
	case eGPUParam::CamPosition:
		mMaterialConstantBuffer.CamPosition = *static_cast<Vector3*>(data);
		break;
	case eGPUParam::Vector4_1:
		mMaterialConstantBuffer.xyzw1 = *static_cast<Vector4*>(data);
		break;
	case eGPUParam::Vector4_2:
		mMaterialConstantBuffer.xyzw2 = *static_cast<Vector4*>(data);
		break;
	case eGPUParam::Vector4_3:
		mMaterialConstantBuffer.xyzw3 = *static_cast<Vector4*>(data);
		break;
	case eGPUParam::Vector4_4:
		mMaterialConstantBuffer.xyzw4 = *static_cast<Vector4*>(data);
		break;
	case eGPUParam::Matrix_1:
		mMaterialConstantBuffer.matrix1 = *static_cast<Matrix*>(data);
		break;
	case eGPUParam::Matrix_2:
		mMaterialConstantBuffer.matrix2 = *static_cast<Matrix*>(data);
		break;
	case eGPUParam::Matrix_3:
		mMaterialConstantBuffer.matrix3 = *static_cast<Matrix*>(data);
		break;
	case eGPUParam::Matrix_4:
		mMaterialConstantBuffer.matrix4 = *static_cast<Matrix*>(data);
		break;
	case eGPUParam::bAlbedo:
		mMaterialConstantBuffer.bAlbedo = *static_cast<int*>(data);
		break;
	case eGPUParam::bNormal:
		mMaterialConstantBuffer.bNormal = *static_cast<int*>(data);
		break;
	case eGPUParam::bMetallic:
		mMaterialConstantBuffer.bMetallic = *static_cast<int*>(data);
		break;
	case eGPUParam::bRoughness:
		mMaterialConstantBuffer.bRoughness = *static_cast<int*>(data);
		break;
	case eGPUParam::bEmissive:
		mMaterialConstantBuffer.bEmissive = *static_cast<int*>(data);
		break;
	case eGPUParam::Bool_1:
		mMaterialConstantBuffer.bool1 = *static_cast<int*>(data);
		break;
	case eGPUParam::Bool_2:
		mMaterialConstantBuffer.bool2 = *static_cast<int*>(data);
		break;
	case eGPUParam::Bool_3:
		mMaterialConstantBuffer.bool3 = *static_cast<int*>(data);
		break;
	default:
		break;
	}

}

void Material::Bind()
{
	for (UINT i = 0; i < static_cast<UINT>(eTextureSlot::End); i++)
	{
		if (nullptr == mTexture[i])
			continue;

		mTexture[i]->BindAllShaderResource(i);
	}

	if (mTexture[static_cast<UINT>(eTextureSlot::Albedo)])
	{
		mMaterialConstantBuffer.bAlbedo = 1;
	}
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::Normal)])
	{
		mMaterialConstantBuffer.bNormal = 1;
	}
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::Metallic)])
	{
		mMaterialConstantBuffer.bMetallic = 1;
	}
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::Roughness)])
	{
		mMaterialConstantBuffer.bRoughness = 1;
	}

	ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];
	pCB->SetData(&mMaterialConstantBuffer);
	pCB->Bind(eShaderStage::VS);
	pCB->Bind(eShaderStage::GS);
	pCB->Bind(eShaderStage::PS);

	mShader->Bind();
}

void Material::Clear()
{
	for (UINT i = 0; i < static_cast<UINT>(eTextureSlot::End); i++)
	{
		if (mTexture[i] == nullptr)
			continue;

		Texture::Clear(i);
	}
}
void Material::SetShaderByKey(std::wstring key)
{
	Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(key);
	mShader = shader;
}

void Material::SetTextureByKey(std::wstring key, eTextureSlot slot)
{
	Texture* texture = GETSINGLE(ResourceMgr)->Find<Texture>(key);
	mTexture[static_cast<UINT>(slot)] = texture;

}



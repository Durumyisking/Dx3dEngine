#include "Material.h"
#include "GameObj.h"


Material::Material()
	:Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{
}
Material::Material(std::wstring textureColor, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

<<<<<<< HEAD
	mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
=======
	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
>>>>>>> origin
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

}
Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

<<<<<<< HEAD
	mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::NormalTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
=======
	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
>>>>>>> origin
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

<<<<<<< HEAD
	mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::NormalTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::MetallicTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
=======
	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	
>>>>>>> origin
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

<<<<<<< HEAD
	mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::NormalTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::MetallicTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::RoughnessTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
=======
	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::Roughness)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
	
>>>>>>> origin
	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

}

Material::Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName)
	: Resource(eResourceType::Material)
	, mMode(eRenderingMode::Opaque)
	, mMaterialConstantBuffer{}
	, mShader(nullptr)
	, mTexture{}
{

<<<<<<< HEAD
	mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::NormalTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::MetallicTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::RoughnessTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
	mTexture[static_cast<UINT>(eTextureSlot::EmissiveTexture)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureEmissive);
=======
	mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureColor);
	mTexture[static_cast<UINT>(eTextureSlot::Normal)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureNormal);
	mTexture[static_cast<UINT>(eTextureSlot::Metallic)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureMetal);
	mTexture[static_cast<UINT>(eTextureSlot::Roughness)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureRoughness);
	mTexture[static_cast<UINT>(eTextureSlot::Emissive)] = GETSINGLE(ResourceMgr)->Find<Texture>(textureEmissive);
>>>>>>> origin

	mShader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);

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
	case eGPUParam::Vector3_3:
		mMaterialConstantBuffer.xyz3 = *static_cast<Vector3*>(data);
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
	case eGPUParam::bTextureExistence:
		mMaterialConstantBuffer.bTextureExistence = *static_cast<int*>(data);
		break;
<<<<<<< HEAD
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
=======
	case eGPUParam::bmetallic:
		mMaterialConstantBuffer.bmetallic = *static_cast<int*>(data);
>>>>>>> origin
		break;
	case eGPUParam::Bool_2:
		mMaterialConstantBuffer.bool2 = *static_cast<int*>(data);
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

		mTexture[i]->BindShaderResource(eShaderStage::VS, i);
		mTexture[i]->BindShaderResource(eShaderStage::HS, i);
		mTexture[i]->BindShaderResource(eShaderStage::DS, i);
		mTexture[i]->BindShaderResource(eShaderStage::GS, i);
		mTexture[i]->BindShaderResource(eShaderStage::PS, i);
		mTexture[i]->BindShaderResource(eShaderStage::CS, i);
	}
<<<<<<< HEAD
=======

	BindingTextures();	

	mShader->Bind();
}
void Material::Clear()
{
	for (size_t i = 0; i < static_cast<UINT>(eTextureSlot::End); i++)
	{
		if (mTexture[i] == nullptr)
			continue;

		mTexture[i]->Clear();
	}

	ConstantBuffer* pCB = renderer::constantBuffers[static_cast<UINT>(eCBType::Material)];
	pCB->Clear();

}
void Material::SetShaderByKey(std::wstring key)
{
	Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(key);
	mShader = shader;
}

void Material::BindingTextures()
{
	ConstantBuffer* pMaterialCB = renderer::constantBuffers[static_cast<UINT>(eCBType::Material)];
>>>>>>> origin

	if (mTexture[static_cast<UINT>(eTextureSlot::ColorTexture)])
		mMaterialConstantBuffer.bAlbedo = 1;
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::NormalTexture)])
		mMaterialConstantBuffer.bNormal = 1;
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::MetallicTexture)])
		mMaterialConstantBuffer.bMetallic = 1;
	if (mTexture[static_cast<UINT>((UINT)eTextureSlot::RoughnessTexture)])
		mMaterialConstantBuffer.bRoughness = 1;

<<<<<<< HEAD
	ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];
	pCB->SetData(&mMaterialConstantBuffer);
	pCB->Bind(eShaderStage::VS);
	pCB->Bind(eShaderStage::GS);
	pCB->Bind(eShaderStage::PS);

	mShader->Bind();
}

void Material::Clear()
{
	for (size_t i = 0; i < static_cast<UINT>(eTextureSlot::End); i++)
	{
		if (mTexture[i] == nullptr)
			continue;

		mTexture[i]->Clear();
	}
}
void Material::SetShaderByKey(std::wstring key)
{
	Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(key);
	mShader = shader;
=======
	if (mTexture[0]) // color가 있습니다.
	{
		++mMaterialConstantBuffer.bTextureExistence;
	}
	if (mTexture[1]) // normal이 있습니다.
	{
		++mMaterialConstantBuffer.bTextureExistence;
	}
	if (mTexture[2]) // Metal이 있습니다.
	{
		++mMaterialConstantBuffer.bTextureExistence;
	}
	if (mTexture[3]) // Roughness가 있습니다.
	{
		++mMaterialConstantBuffer.bTextureExistence;
	}
	if (mTexture[4]) // Emissive가 있습니다.
	{
		++mMaterialConstantBuffer.bTextureExistence;
	}

//	mMaterialConstantBuffer.CamPosition = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetWorldPosition();

	pMaterialCB->SetData(&mMaterialConstantBuffer);
	pMaterialCB->Bind(eShaderStage::VS);
	pMaterialCB->Bind(eShaderStage::GS);
	pMaterialCB->Bind(eShaderStage::PS);
>>>>>>> origin
}

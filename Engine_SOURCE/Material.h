#pragma once
#include "EngineResource.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

using namespace renderer;



class Material : public Resource
{
public:
	Material();
	Material(std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName);

	Material(std::wstring textureName, eTextureSlot slot, std::wstring shaderName);

	virtual ~Material();

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;

	void SetData(eGPUParam param, void* data);
	void Bind();
	void Clear();

	void SetShader(Shader* shader) 
	{	
		mShader = shader; 

		assert(mShader);
	
		if (L"DeferredShader" == mShader->GetName())
		{
			SetRenderingMode(eRenderingMode::DeferredOpaque);
		}
	}
	void SetShaderByKey(std::wstring key);
	Shader* GetShader() const { return mShader; }

	void SetTexture(Texture* texture) { mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = texture; }
	void SetTexture(eTextureSlot slot, Texture* texture) { mTexture[static_cast<UINT>(slot)] = texture; }

	void SetTextureByKey(std::wstring key, eTextureSlot slot);

	Texture* GetTexture() const { return mTexture[static_cast<UINT>(eTextureSlot::Albedo)]; }
	Texture* GetTexture(eTextureSlot slot) const { return mTexture[static_cast<UINT>(slot)]; }

	eRenderingMode GetRenderingMode() const { return mMode; }
	void SetRenderingMode(eRenderingMode mode) { mMode = mode; }

	void SetMetallic(const float metallic) { mMaterialConstantBuffer.metallic = metallic; }
	void SetRoughness(const float roughness) { mMaterialConstantBuffer.roughness= roughness; }
	void SetFresnelCoeff (const Vector3 value) { mMaterialConstantBuffer.FresnelCoeff = value; }

	float GetMetallic() const { return mMaterialConstantBuffer.metallic; }
	float GetRoughness() const { return mMaterialConstantBuffer.roughness ; }
	Vector3 GetFresnelCoeff() const { return mMaterialConstantBuffer.FresnelCoeff ; }
	//void BindingTextures();



private:
	Shader* mShader;
	Texture* mTexture[static_cast<UINT>(eTextureSlot::End)];

	MaterialCB			mMaterialConstantBuffer;
	eRenderingMode		mMode;


};


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
	Material(std::wstring textureColor, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName);
	Material(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName);


	Material(std::wstring textureName, eTextureSlot slot, std::wstring shaderName);
	virtual ~Material();

	virtual HRESULT Load(const std::wstring& path) override;

	void SetData(eGPUParam param, void* data);
	void Bind();
	void Clear();

	void SetShader(Shader* shader) { mShader = shader; }
	void SetShaderByKey(std::wstring key);
	Shader* GetShader() const { return mShader; }

	void SetTexture(Texture* texture) { mTexture[static_cast<UINT>(eTextureSlot::Albedo)] = texture; }
	void SetTexture(eTextureSlot slot, Texture* texture) { mTexture[static_cast<UINT>(slot)] = texture; }

	Texture* GetTexture() const { return mTexture[static_cast<UINT>(eTextureSlot::Albedo)]; }
	Texture* GetTexture(eTextureSlot slot) const { return mTexture[static_cast<UINT>(slot)]; }

	eRenderingMode GetRenderingMode() const { return mMode; }
	void SetRenderingMode(eRenderingMode mode) { mMode = mode; }


	void BindingTextures();


private:
	Shader* mShader;
	Texture* mTexture[static_cast<UINT>(eTextureSlot::End)];

	MaterialCB			mMaterialConstantBuffer;
	eRenderingMode		mMode;


};


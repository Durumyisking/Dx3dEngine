#pragma once
#include "EngineResource.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

using namespace dru::renderer;

namespace dru
{
	class Material : public Resource
	{
	public:
		Material();
		Material(std::wstring textureName, std::wstring shaderName);
		Material(std::wstring textureName, eTextureSlot slot, std::wstring shaderName);
		virtual ~Material();

		virtual HRESULT Load(const std::wstring& path) override;

		void SetData(eGPUParam param, void* data);
		void Bind();
		void Clear();

		void SetShader(Shader*	shader) { mShader = shader; }
		void SetShaderByKey(std::wstring key);
		Shader* GetShader() const { return mShader; }

		void SetTexture(Texture* texture) { mTexture[static_cast<UINT>(eTextureSlot::T0)] = texture; }
		void SetTexture(eTextureSlot slot, Texture* texture) { mTexture[static_cast<UINT>(slot)] = texture; }
		Texture* GetTexture() const { return mTexture[static_cast<UINT>(eTextureSlot::T0)]; }
		Texture* GetTexture(eTextureSlot slot) const { return mTexture[static_cast<UINT>(slot)]; }

		eRenderingMode GetRenderingMode() const { return mMode; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }

	private:
		Shader*				mShader;
		Texture*			mTexture[static_cast<UINT>(eTextureSlot::End)];
		MaterialCB			mConstantBuffer;
		eRenderingMode		mMode;


	};

}



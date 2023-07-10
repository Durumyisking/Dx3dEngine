#pragma once
#include "Resource.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

using namespace dru::renderer;

namespace dru::graphics
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

		void SetShader(std::shared_ptr<Shader>	 shader) { mShader = shader; }
		void SetShaderByKey(std::wstring key);
		std::shared_ptr<Shader> GetShader() const { return mShader; }

		void SetTexture(std::shared_ptr<Texture> texture) { mTexture[static_cast<UINT>(eTextureSlot::T0)] = _Texture; }
		void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> texture) { mTexture[static_cast<UINT>(slot)] = _Texture; }
		std::shared_ptr<Texture> GetTexture() const { return mTexture[static_cast<UINT>(eTextureSlot::T0)]; }
		std::shared_ptr<Texture> GetTexture(eTextureSlot slot) const { return mTexture[static_cast<UINT>(slot)]; }

		eRenderingMode GetRenderingMode() const { return mMode; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }

	private:
		std::shared_ptr<Shader>				mShader;
		std::shared_ptr<Texture>			mTexture[static_cast<UINT>(eTextureSlot::End)];
		MaterialCB							mConstantBuffer;
		eRenderingMode						mMode;


	};

}



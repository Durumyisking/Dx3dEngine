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
		Material(std::wstring _TextureName, std::wstring _ShaderName);
		Material(std::wstring _TextureName, eTextureSlot _Slot, std::wstring _ShaderName);
		virtual ~Material();

		virtual HRESULT Load(const std::wstring& path) override;

		void SetData(eGPUParam _Param, void* _Data);
		void Bind();
		void Clear();

		void SetShader(Shader*	_Shader) { mShader = _Shader; }
		void SetShaderByKey(std::wstring _Key);
		Shader* GetShader() const { return mShader; }

		void SetTexture(Texture* _Texture) { mTexture[static_cast<UINT>(eTextureSlot::T0)] = _Texture; }
		void SetTexture(eTextureSlot slot, Texture* _Texture) { mTexture[static_cast<UINT>(slot)] = _Texture; }
		Texture* GetTexture() const { return mTexture[static_cast<UINT>(eTextureSlot::T0)]; }
		Texture* GetTexture(eTextureSlot slot) const { return mTexture[static_cast<UINT>(slot)]; }

		eRenderingMode GetRenderingMode() const { return mMode; }
		void SetRenderingMode(eRenderingMode _Mode) { mMode = _Mode; }

	private:
		Shader*				mShader;
		Texture*			mTexture[static_cast<UINT>(eTextureSlot::End)];
		MaterialCB			mConstantBuffer;
		eRenderingMode		mMode;


	};

}



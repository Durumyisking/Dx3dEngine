#pragma once
#include "Graphics.h"
#include "EngineResource.h"

namespace dru::graphics
{
	class ComputeShader : public Resource
	{
	public:
		ComputeShader(UINT threadGroupX, UINT threadGroupY, UINT threadGroupZ);
		ComputeShader();
		virtual ~ComputeShader();

		virtual HRESULT Load(const std::wstring& path);
		void Create(const std::wstring& path, const std::string& funcName);
		void OnExcute();

		virtual void Bind();
		virtual void Clear();

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob>			mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

		Microsoft::WRL::ComPtr<ID3DBlob>			mPSBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS;
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;


		UINT mThreadGroupCountX;
		UINT mThreadGroupCountY;
		UINT mThreadGroupCountZ;


		UINT mGroupX;
		UINT mGroupY;
		UINT mGroupZ;
	};

}


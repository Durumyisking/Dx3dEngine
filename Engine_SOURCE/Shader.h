#pragma once
#include "EngineResource.h"
#include "Graphics.h"


class Shader : public Resource
{
public:
	Shader();
	virtual ~Shader();

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;


	void Create(eShaderStage eStage, const std::wstring& path, const std::string& funcName);
	void Bind();
	ID3D11InputLayout* GetInputLayOut() { return mInputLayout.Get(); }
	ID3D11InputLayout** GetInputLayoutAddr()  { return mInputLayout.GetAddressOf(); }

	void* GetVSBlobBufferPointer() { return mVSBlob->GetBufferPointer(); }
	SIZE_T GetVSBlobBufferSize() { return mVSBlob->GetBufferSize(); }

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { mTopology = topology; }

	void SetRSState(eRasterizerType state) { mRSType = state; }
	void SetDSState(eDepthStencilType state) { mDSType = state; }
	void SetBSState(eBlendStateType state) { mBSType = state; }

private:
	void CreateVS(const std::wstring& path, const std::string& funcName);
	void CreateHS(const std::wstring& path, const std::string& funcName);
	void CreateDS(const std::wstring& path, const std::string& funcName);
	void CreateGS(const std::wstring& path, const std::string& funcName);
	void CreatePS(const std::wstring& path, const std::string& funcName);
		


private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	D3D11_PRIMITIVE_TOPOLOGY mTopology;
	eShaderStage mCurrentStage;


	Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;

	// Shaders
	Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> mHSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> mDSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> mGSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> mPSBlob;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVS;
	Microsoft::WRL::ComPtr<ID3D11HullShader> mHS;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> mDS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS;

	eRasterizerType		mRSType;
	eDepthStencilType	mDSType;
	eBlendStateType		mBSType;

};





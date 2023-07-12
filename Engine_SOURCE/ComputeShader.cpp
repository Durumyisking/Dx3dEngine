#include "ComputeShader.h"

namespace dru
{
	ComputeShader::ComputeShader(UINT threadGroupX, UINT threadGroupY, UINT threadGroupZ)
		: Resource(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGroupCountX(threadGroupX)
		, mThreadGroupCountY(threadGroupY)
		, mThreadGroupCountZ(threadGroupZ)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
	}

	ComputeShader::ComputeShader()
		: Resource(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGroupCountX(0)
		,mThreadGroupCountY(0)
		,mThreadGroupCountZ(0)
		,mGroupX(0)
		,mGroupY(0)
		,mGroupZ(0)
	{
		mThreadGroupCountX = 32;
		mThreadGroupCountY = 32;
		mThreadGroupCountZ = 1;
	}

	ComputeShader::~ComputeShader()
	{
	}

	HRESULT ComputeShader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	void ComputeShader::Create(const std::wstring& path, const std::string& funcName)
	{

		mErrorBlob = nullptr;

		std::filesystem::path filepath = std::filesystem::current_path().parent_path();
		filepath += "\\..\\SHADER_SOURCE\\";

		std::wstring shaderPath(filepath.c_str());
		shaderPath += path;


		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, funcName.c_str(), "cs_5_0", 0, 0, mCSBlob.GetAddressOf(), mErrorBlob.GetAddressOf());

		if (mErrorBlob)
		{
			OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			mErrorBlob->Release();
			mErrorBlob = nullptr;
		}


		dru::GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer()
			, mCSBlob->GetBufferSize()
			, nullptr
			, mCS.GetAddressOf());

	}



	void ComputeShader::OnExcute()
	{
		Bind();	

		GetDevice()->BindCS(mCS.Get(), nullptr, 0);
		// 컴퓨트 쉐이더 돌릴거임, 인자로 쓰레드 그룹 전달
		GetDevice()->Dispatch(mGroupX, mGroupY, mGroupZ);

		Clear();
	}

	void ComputeShader::Bind()
	{
	}

	void ComputeShader::Clear()
	{
	}

}
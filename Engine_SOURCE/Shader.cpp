#include "Shader.h"
#include "GraphicDevice.h"
#include "Renderer.h"

namespace dru
{
	Shader::Shader()
		: Resource(eResourceType::GraphicShader)
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRasterizerType::SolidBack)
		, mDSType(eDepthStencilType::Less)
		, mBSType(eBlendStateType::AlphaBlend)
		, mCurrentStage(eShaderStage::End)
	{
	}
	Shader::~Shader()
	{
	}

	HRESULT Shader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}



	void Shader::Create(dru::eShaderStage _eStage, const std::wstring& _Path, const std::string& _funcName)
	{
		mErrorBlob = nullptr;

		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\..\\SHADER_SOURCE\\";

		std::wstring shaderPath(path.c_str());
		shaderPath += _Path;


		switch (_eStage)
		{
		case dru::eShaderStage::VS:
			CreateVS(shaderPath, _funcName);
			break;
		case dru::eShaderStage::HS:
			CreateHS(shaderPath, _funcName);
			break;
		case dru::eShaderStage::DS:
			CreateDS(shaderPath, _funcName);
			break;
		case dru::eShaderStage::GS:	
			CreateGS(shaderPath, _funcName);
			break;
		case dru::eShaderStage::PS:
			CreatePS(shaderPath, _funcName);
			break;
		case dru::eShaderStage::CS:
			break;
		case dru::eShaderStage::End:
			break;
		default:
			break;
		}

	}

	void Shader::CreateVS(const std::wstring& _Path, const std::string& _funcName)
	{
		D3DCompileFromFile(_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _funcName.c_str(), "vs_5_0", 0, 0, mVSBlob.GetAddressOf(), mErrorBlob.GetAddressOf());

		if (mErrorBlob)
		{
			OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			mErrorBlob->Release();
			mErrorBlob = nullptr;
		}
		
		dru::GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer()
			, mVSBlob->GetBufferSize()
			, nullptr
			, mVS.GetAddressOf());

	}

	void Shader::CreateHS(const std::wstring& _Path, const std::string& _funcName)
	{

	}

	void Shader::CreateDS(const std::wstring& _Path, const std::string& _funcName)
	{

	}

	void Shader::CreateGS(const std::wstring& _Path, const std::string& _funcName)
	{
		D3DCompileFromFile(_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _funcName.c_str(), "gs_5_0", 0, 0, mGSBlob.GetAddressOf(), mErrorBlob.GetAddressOf());

		if (mErrorBlob)
		{
			OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			mErrorBlob->Release();
			mErrorBlob = nullptr;
		}


		GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer()
			, mGSBlob->GetBufferSize()
			, nullptr
			, mGS.GetAddressOf());
	}

	void Shader::CreatePS(const std::wstring& _Path, const std::string& _funcName)
	{
		D3DCompileFromFile(_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _funcName.c_str(), "ps_5_0", 0, 0, mPSBlob.GetAddressOf(), mErrorBlob.GetAddressOf());

		if (mErrorBlob)
		{
			OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			mErrorBlob->Release();
			mErrorBlob = nullptr;
		}

		dru::GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer()
			, mPSBlob->GetBufferSize()
			, nullptr
			, mPS.GetAddressOf());
	}


	void Shader::Bind()
	{
		dru::GetDevice()->BindPrimitiveTopology(mTopology);
		dru::GetDevice()->BindInputLayout(mInputLayout.Get());

		dru::GetDevice()->BindVS(mVS.Get(), nullptr, 0);
		dru::GetDevice()->BindHS(mHS.Get(), nullptr, 0);
		dru::GetDevice()->BindDS(mDS.Get(), nullptr, 0);
		dru::GetDevice()->BindGS(mGS.Get(), nullptr, 0);
		dru::GetDevice()->BindPS(mPS.Get(), nullptr, 0);

		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rs = renderer::rasterizerState	[static_cast<UINT>(mRSType)];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	ds = renderer::depthStencilState[static_cast<UINT>(mDSType)];
		Microsoft::WRL::ComPtr<ID3D11BlendState>		bs = renderer::blendState		[static_cast<UINT>(mBSType)];

		GetDevice()->BindRasterizerState(rs.Get());
		GetDevice()->BindDepthStencilState(ds.Get());
		GetDevice()->BindBlendState(bs.Get());


	}

}
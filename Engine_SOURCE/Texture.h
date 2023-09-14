#pragma once
#include "EngineResource.h"
#include "ResourceMgr.h"

#pragma warning( push )
#pragma warning( disable : 4099 )
#include "../External/DirectXTex/include/DirectXTex.h"
#pragma warning( pop ) 

#include "GraphicDevice.h"
#include "Model.h"

#ifdef _DEBUG
#pragma comment(lib, "../External/DirectXTex/lib/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "../External/DirectXTex/lib/Release/DirectXTex.lib")
#endif





class Texture : public Resource
{
public:
	Texture();
	virtual ~Texture();

	static void Clear(UINT startSlot);
	static void Clears();

	bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindflag);
	bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT numQualityLevels, UINT bindflag);
	bool Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);
	bool Create(D3D11_TEXTURE2D_DESC& desc);

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;
	Texture* Load(const std::wstring& path, const Model::TextureInfo& info);


	void BindShaderResource(eShaderStage stage, UINT slot);
	void BindShaderResource_VP(UINT slot);
	void BindAllShaderResource(UINT slot);
	void BindUnorderedAccessview(UINT slot);
	void ClearUnorderedAccessview(UINT slot);


	void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture) { mTexture = texture; }
	//void MakeCropTexture(UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox);
	ScratchImage& GetScratchImage() { return mImage; }

	UINT GetHeight() { return mDesc.Height; }
	UINT GetWidth() { return mDesc.Width; }



	Microsoft::WRL::ComPtr<ID3D11Texture2D>	GetTexture()		{ return mTexture; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDSV()		{ return mDSV; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV()	{ return mSRV; }
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV()		{ return mRTV; }
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV()	{ return mUAV; }

private:
	ScratchImage										mImage;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				mTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		mDSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		mRTV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	mUAV;
	D3D11_TEXTURE2D_DESC								mDesc;

};



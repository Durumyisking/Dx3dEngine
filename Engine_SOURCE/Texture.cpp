#include "Texture.h"




Texture::Texture()
	: Resource(eResourceType::Texture)
	, mDesc{}
	, mTexture(nullptr)
		
{
}

Texture::~Texture()
{
}

void Texture::Clear(UINT startSlot)
{
	ID3D11ShaderResourceView* srv = nullptr;

	GetDevice()->BindShaderResource(eShaderStage::VS, startSlot, &srv);
	GetDevice()->BindShaderResource(eShaderStage::DS, startSlot, &srv);
	GetDevice()->BindShaderResource(eShaderStage::GS, startSlot, &srv);
	GetDevice()->BindShaderResource(eShaderStage::HS, startSlot, &srv);
	GetDevice()->BindShaderResource(eShaderStage::CS, startSlot, &srv);
	GetDevice()->BindShaderResource(eShaderStage::PS, startSlot, &srv);
}

bool Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindflag)
{
	mDesc.BindFlags = bindflag;
	mDesc.Usage = D3D11_USAGE_DEFAULT;
	mDesc.CPUAccessFlags = 0;
	mDesc.Format = format;
	mDesc.Width = width;
	mDesc.Height = height;
	mDesc.ArraySize = 1;

	mDesc.SampleDesc.Count = 1;
	mDesc.SampleDesc.Quality = 0;

	mDesc.MipLevels = 1;
	mDesc.MiscFlags = 0;

	if (!GetDevice()->CreateTexture(&mDesc, mTexture.GetAddressOf()))
	{
		return false;
	}
	if (bindflag & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
		{
			return false;
		}
	}
	if (bindflag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC tSRVdesc = {};
		tSRVdesc.Format = format;
		tSRVdesc.Texture2D.MipLevels = 0;
		tSRVdesc.Texture2D.MostDetailedMip = 0;
		tSRVdesc.ViewDimension = D3D11_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2D;

		if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
		{
			return false;
		}
	}
	if (bindflag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVdesc = {};
		tUAVdesc.Format = format;
		tUAVdesc.Texture2D.MipSlice = 0;
		tUAVdesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

		if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
		{
			return false;
		}
	}
	if (bindflag & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		if (!GetDevice()->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf()))
			return false;
	}


	return true;
}

bool Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
{
	mTexture = texture;
	mTexture->GetDesc(&mDesc);

	if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
		{
			return false;
		}
	}
	if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		if (!GetDevice()->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf()))
		{
			return false;
		}
	}
	if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC tSRVdesc = {};
		tSRVdesc.Format = mDesc.Format;
		tSRVdesc.Texture2D.MipLevels = 0;
		tSRVdesc.Texture2D.MostDetailedMip = 0;
		tSRVdesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

		if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
		{
			return false;
		}
	}
	if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVdesc = {};
		tUAVdesc.Format = mDesc.Format;
		tUAVdesc.Texture2D.MipSlice = 0;
		tUAVdesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

		if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
		{
			return false;
		}
	}

	return true;
}

HRESULT Texture::Load(const std::wstring& path)
{
	std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
	std::wstring fullPath = parentPath.wstring() + L"/../Resources/" + path;
			

	wchar_t szExtension[256] = {};
	_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // 경로에서 확장자만 뽑아오는 녀석

	std::wstring extension(szExtension);

	if (extension == L".dds" || extension == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(fullPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
			return S_FALSE;
	}
	else if (extension == L".tga" || extension == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(fullPath.c_str(), nullptr, mImage)))
			return S_FALSE;
	}
	else // WIC (png, jpg, jpeg, bmp )
	{
		if (FAILED(LoadFromWICFile(fullPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
			return S_FALSE;
	}


	CreateShaderResourceView(
		GetDevice()->GetID3D11Device()
		, mImage.GetImages()
		, mImage.GetImageCount()
		, mImage.GetMetadata()
		, mSRV.GetAddressOf()
	);


	mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

	mTexture->GetDesc(&mDesc);


	return S_OK;
}

Texture* Texture::Load(const std::wstring& path, const Model::TextureInfo& info)
{
	wchar_t szExtension[256] = {};
	_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // 경로에서 확장자만 뽑아오는 녀석

	std::wstring extension(szExtension);

	if (extension == L".dds" || extension == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
			return nullptr;
	}
	else if (extension == L".tga" || extension == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(path.c_str(), nullptr, mImage)))
			return nullptr;
	}
	else // WIC (png, jpg, jpeg, bmp )
	{
		if (FAILED(LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
			return nullptr;
	}


	CreateShaderResourceView(
		GetDevice()->GetID3D11Device()
		, mImage.GetImages()
		, mImage.GetImageCount()
		, mImage.GetMetadata()
		, mSRV.GetAddressOf()
	);


	mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

	mTexture->GetDesc(&mDesc);
	
	SetName(info.texName);
	SetPath(info.texPath);

	return this;
}

void Texture::BindShaderResource(eShaderStage stage, UINT slot)
{
	GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
}

void Texture::BindShaderResource_VP(UINT slot)
{
	GetDevice()->BindShaderResource(eShaderStage::VS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::PS, slot, mSRV.GetAddressOf());
}

void Texture::BindAllShaderResource(UINT slot)
{
	GetDevice()->BindShaderResource(eShaderStage::VS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::DS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::GS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::HS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::CS, slot, mSRV.GetAddressOf());
	GetDevice()->BindShaderResource(eShaderStage::PS, slot, mSRV.GetAddressOf());
}

void Texture::BindUnorderedAccessview(UINT slot)
{
	UINT i = -1; 
	GetDevice()->BindUnorderedAccessView(slot, 1, mUAV.GetAddressOf(), &i);
}

void Texture::ClearUnorderedAccessview(UINT slot)
{
	ID3D11UnorderedAccessView* p = nullptr;
	UINT i = -1;
	GetDevice()->BindUnorderedAccessView(slot, 1, &p, &i);
}


void Texture::Clear()
{
	ID3D11ShaderResourceView* srv = nullptr;

	GetDevice()->BindShaderResource(eShaderStage::VS, 0, &srv);
	GetDevice()->BindShaderResource(eShaderStage::DS, 0, &srv);
	GetDevice()->BindShaderResource(eShaderStage::GS, 0, &srv);
	GetDevice()->BindShaderResource(eShaderStage::HS, 0, &srv);
	GetDevice()->BindShaderResource(eShaderStage::CS, 0, &srv);
	GetDevice()->BindShaderResource(eShaderStage::PS, 0, &srv);
}


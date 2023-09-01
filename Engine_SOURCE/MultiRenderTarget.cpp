#include "MultiRenderTarget.h"

MultiRenderTarget::MultiRenderTarget()
	: mRenderTargets{}
	, mDSTexture(nullptr)
	, mRenderTargetCount(0)
	, mClearColors{}
{
	mRenderTargets.resize(12);
}

MultiRenderTarget::~MultiRenderTarget()
{
}

void MultiRenderTarget::Create(std::vector<Texture*> textures, Texture* dsTexture)
{
	for (UINT i = 0; i <= textures.size(); i++)
	{
		if (i == textures.size())
		{
			mRenderTargetCount = i;
			break;
		}

		mRenderTargets[i] = textures[i];
	}

	mDSTexture = dsTexture;
}

void MultiRenderTarget::Create(Texture* textures, Texture* dsTexture)
{
}

void MultiRenderTarget::OMSetRenderTarget()
{
	Texture::Clears();

	ID3D11RenderTargetView* arrRenderTargetViews[12] = {};
	for (UINT i = 0; i < 12; i++)
	{
		if (mRenderTargets[i])
		{
			arrRenderTargetViews[i] = mRenderTargets[i]->GetRTV().Get(); // 텍스처 만들때 플래그 설정 해두어서 렌더타겟으로 사용 가능
		}
	}

	if (mDSTexture != nullptr)
	{
		GetDevice()->OMSetRenderTarget(mRenderTargetCount, arrRenderTargetViews, mDSTexture->GetDSV().Get());
	}
	else
	{
		GetDevice()->OMSetRenderTarget(mRenderTargetCount, arrRenderTargetViews, nullptr);
	}
}

void MultiRenderTarget::Clear(FLOAT backgroundColor[4] )
{
	//FLOAT backgroundColor[4] = backgroundColor;

	for (UINT i = 0; i < mRenderTargetCount; i++)
	{
		if (mRenderTargets[i])
		{
			GetDevice()->ClearRenderTargetView(mRenderTargets[i]->GetRTV().Get(), backgroundColor);
		}
	}

	if (mDSTexture != nullptr)
	{
		GetDevice()->ClearDepthStencilView(mDSTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
	}
}

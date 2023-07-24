#include "MultiRenderTarget.h"

MultiRenderTarget::MultiRenderTarget()
	: mRenderTargets{}
	, mDSTexture(nullptr)
	, mRenderTargetCount(0)
	, mClearColors{}
{
}

MultiRenderTarget::~MultiRenderTarget()
{
}

void MultiRenderTarget::Create(Texture* texture[8], Texture* dsTexture)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (texture[i] == nullptr)
		{
			mRenderTargetCount = i;
			break;
		}

		mRenderTargets[i] = texture[i];
	}

	mDSTexture = dsTexture;
}

void MultiRenderTarget::OMSetRenderTarget()
{
	ID3D11RenderTargetView* arrRenderTargetViews[8] = {};
	for (size_t i = 0; i < 8; i++)
	{
		if (mRenderTargets[i])
		{
			arrRenderTargetViews[i] = mRenderTargets[i]->GetRTV().Get();
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

void MultiRenderTarget::Clear()
{
	FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	for (size_t i = 0; i < mRenderTargetCount; i++)
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

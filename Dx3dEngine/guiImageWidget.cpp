#include "guiImageWidget.h"
#include "ResourceMgr.h"

namespace gui
{
	ImageWidget::ImageWidget()
		: Widget()
		, mTexture(nullptr)
		, mImageStart{0.f, 0.f}
		, mImageEnd{100.f, 100.f}
		, mBorderColor{}
	{
	}
	ImageWidget::ImageWidget(std::string name)
		: Widget()
		, mTexture(nullptr)
		, mImageStart{ 0.f, 0.f }
		, mImageEnd{ 100.f, 100.f }
		, mBorderColor{}
	{
		SetName(name);
	}
	ImageWidget::~ImageWidget()
	{
	}

	void ImageWidget::Update()
	{
		if (mTexture == nullptr)
			return;

		unsigned int	width = mTexture->GetWidth();
		unsigned int	height = mTexture->GetHeight();

		ImVec2 StartUV = ImVec2(mImageStart.x / width, mImageStart.y / height);
		ImVec2 EndUV = ImVec2(mImageEnd.x / width, mImageEnd.y / height);

		ImGui::Image(mTexture->GetSRV().Get(), mSize, StartUV, EndUV, mColor, mBorderColor);
	}

	void ImageWidget::SetTexture(Texture* texture)
	{
		mTexture = texture;

		if (mTexture == nullptr)
		{
			mTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
		}

		mImageEnd.x = (float)mTexture->GetWidth();
		mImageEnd.y = (float)mTexture->GetHeight();
	}
	void ImageWidget::SetTexture(const std::wstring& name)
	{
		mTexture = GETSINGLE(ResourceMgr)->Find<Texture>(name);

		if (mTexture == nullptr)
		{
			mTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
		}

		mImageEnd.x = (float)mTexture->GetWidth();
		mImageEnd.y = (float)mTexture->GetHeight();

		mColor = ImVec4(1.f, 1.f, 1.f, 1.f);
	}
}
#pragma once
#include "guiWidget.h"
#include "Texture.h"

namespace gui
{
    class ImageWidget :
        public Widget
    {   
	public:
		ImageWidget();
		ImageWidget(std::string name);
		virtual ~ImageWidget();

		virtual void Update() override;

		void SetImageStart(float x, float y)
		{
			mImageStart.x = x;
			mImageStart.y = y;
		}

		void SetImageEnd(float x, float y)
		{
			mImageEnd.x = x;
			mImageEnd.y = y;
		}

		void SetBorderColor(unsigned char r, unsigned char g, unsigned char b,
			unsigned char a)
		{
			mBorderColor.x = r / 255.f;
			mBorderColor.y = g / 255.f;
			mBorderColor.z = b / 255.f;
			mBorderColor.w = a / 255.f;
		}

		void SetTexture(Texture* texture);
		void SetTexture(const std::wstring& name);

	private:
		Texture*	mTexture;
		ImVec2		mImageStart;
		ImVec2		mImageEnd;
		ImVec4		mBorderColor;
    };
}


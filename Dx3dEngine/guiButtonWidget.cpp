#include "guiButtonWidget.h"
#include "ResourceMgr.h"

namespace gui
{
	ButtonWidget::ButtonWidget()
		: Widget()
		, mClick()
		, mbText(false)
		, mText()
		, mButtonText()
		, mTexture(nullptr)
	{
	}
	ButtonWidget::ButtonWidget(std::string name)
		: Widget()
		, mClick()
		, mbText(false)
		, mText()
		, mButtonText()
		, mTexture(nullptr)
	{
		SetName(name);
	}
	ButtonWidget::~ButtonWidget()
	{
		mTexture = nullptr;
	}

	void ButtonWidget::FixedUpdate()
	{
	}

	void ButtonWidget::Update() 
	{
		if (mbText)
		{
			if (mTexture == nullptr)
			{
				if (mButtonText == "")
					mClick = ImGui::Button(GetName().c_str(), ImVec2(mSize.x - 25.f, mSize.y - 25.f));
				else
					mClick = ImGui::Button(mButtonText.c_str(), ImVec2(mSize.x - 25.f, mSize.y - 25.f));
			}
			else
				mClick = ImGui::ImageButton(mTexture->GetSRV().Get(), ImVec2(mSize.x - 25.f, mSize.y - 25.f));

			ImGui::Text(mText.c_str());
		}
		else
		{
			if (mTexture == nullptr)
				mClick = ImGui::Button(GetName().c_str(), mSize);
			else
				mClick = ImGui::ImageButton(mTexture->GetSRV().Get(), mSize);
		}

		//mClick = ImGui::Button(GetName().c_str(), mSize);

		if (mClick && mClickCallback)
		{
			mClickCallback();
		}
	}

	void ButtonWidget::LateUpdate()
	{
	}

	void ButtonWidget::SetTexture(Texture* tex)
	{
		mTexture = tex;
	}

	void ButtonWidget::SetTexture(const std::wstring& name)
	{
		mTexture = GETSINGLE(ResourceMgr)->Find<Texture>(name);
	}

}
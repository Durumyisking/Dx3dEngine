#include "guiButtonWidget.h"
#include "ResourceMgr.h"

namespace gui
{
	ButtonWidget::ButtonWidget()
		: Widget()
		, mClick()
		, mbText(false)
		, mText()
		, mTexture(nullptr)
	{
	}
	ButtonWidget::ButtonWidget(std::string name)
		: Widget()
		, mClick()
		, mbText(false)
		, mText()
		, mTexture(nullptr)
	{
		SetName(name);
		SetSize(100.f, 100.f);
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
				mClick = ImGui::Button(GetName().c_str(), ImVec2(mSize.x - 25.f, mSize.y - 25.f));
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

	void ButtonWidget::SetTexture(Texture* tex, float sizeX, float sizeY)
	{
		mTexture = tex;
		mSize = ImVec2(sizeX, sizeY);
	}

	void ButtonWidget::SetTexture(std::wstring& name, float sizeX, float sizeY)
	{
		GETSINGLE(ResourceMgr)->Find<Texture>(name);
		mSize = ImVec2(sizeX, sizeY);
	}

}
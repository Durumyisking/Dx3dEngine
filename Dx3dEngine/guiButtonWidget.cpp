#include "guiButtonWidget.h"
#include "ResourceMgr.h"

namespace gui
{
	ButtonWidget::ButtonWidget()
		: Widget()
		//, mClick()
		//, mbText(false)
		//, mText()
		//, mTexture(nullptr)
	{
	}
	ButtonWidget::ButtonWidget(std::string name)
		: Widget()
		//, mClick()
		//, mbText(false)
		//, mText()
		//, mTexture(nullptr)
	{
		SetName(name);
		SetSize(100.f, 100.f);
	}
	ButtonWidget::~ButtonWidget()
	{
	}

	void ButtonWidget::FixedUpdate()
	{
	}

	void ButtonWidget::Update()
	{
		if (mTexture == nullptr)
			mClick = ImGui::Button(GetName().c_str(), mSize);
		else
			mClick = ImGui::ImageButton(mTexture, mSize);

		if (mbText)
			ImGui::Text(mText.c_str());

		//mClick = ImGui::Button(GetName().c_str(), mSize);

		if (mClick && mCallback)
		{
			mCallback();
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
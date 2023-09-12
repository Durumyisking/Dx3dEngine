#include "guiWidget.h"

namespace gui
{
	Widget::Widget()
		: GuiEntity()
		, mState(eState::Active)
		, mSize{}
		, mColor{}
		, mParent(nullptr)
		, mChilds{}
		, mWindow_flags(ImGuiWindowFlags_None)
	{
	}

	Widget::~Widget()
	{
	}

	void Widget::FixedUpdate()
	{
		//if (mState != eState::Active)
		//	return;
	}

	void Widget::Update()
	{
		//if (mState != eState::Active)
		//	return;
	}

	void Widget::LateUpdate()
	{
		//if (mState != eState::Active)
		//	return;
	}

	void Widget::Render()
	{
		if (mState != eState::Active)
			return;

		// 최상위부모 일때
		if (mParent == nullptr) 
		{
			bool open = (bool)GetState();
			FixedUpdate();
			if (!ImGui::Begin(GetName().c_str(), &open, mWindow_flags))
				mState = eState::Paused;
			Update();
			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();

			ImGui::End();
		}
		else
		{
			FixedUpdate();
			ImGui::BeginChild(GetName().c_str(), mSize);
			//size 추가 해줘야한다.-
			Update();
			std::string name = GetName();
			UINT size = mChilds.size();
			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();
			//std::string debug = GetName();
			ImGui::EndChild();
		}
	}

	void Widget::Close()
	{
	}

	void Widget::AddWidget(Widget* widget)
	{
		mChilds.push_back(widget);
		widget->SetParent(this);
	}

	void Widget::WindowFocus(bool enable)
	{
		if (enable == true)
			ImGui::SetWindowFocus(GetName().c_str());
		else
			ImGui::SetWindowFocus(nullptr);
	}
}

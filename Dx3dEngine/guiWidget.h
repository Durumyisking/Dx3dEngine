#pragma once
#include "Entity.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


namespace gui
{
	class Widget : public GuiEntity
	{
	public:
		enum class eState
		{
			Paused,
			Active,
			Dead,
		};

		Widget();
		virtual ~Widget();

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Close();

		void AddWidget(Widget* widget);
		void WindowFocus(bool enable);

		void SetState(eState state) { mState = state; }
		eState GetState() { return mState; }
		void SetParent(Widget* parent) { mParent = parent; }
		Widget* GetParent() { return mParent; }
		void SetSize(ImVec2 size) { mSize = size; }
		void SetSize(float x, float y) { mSize = ImVec2(x, y); }
		ImVec2 GetSize() { return mSize; }
		void SetColor(ImVec4 color) { mColor = color; }
		void SetColor(float r, float g, float b, float a) { mColor = ImVec4(r, g, b, a); }

	protected:
		ImGuiWindowFlags mWindow_flags;
		ImVec2 mSize;
		ImVec4 mColor;
		eState mState;
		std::vector<Widget*> mChilds;

	private:
		Widget* mParent;
	};
}
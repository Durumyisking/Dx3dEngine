#pragma once
#include "guiWidget.h"
#include "Texture.h"


namespace gui
{
	class ButtonWidget :
		public Widget
	{
	public:
		ButtonWidget();
		ButtonWidget(std::string name);
		virtual ~ButtonWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetText(const std::string& text) { mText = text; mbText = true; }

		void SetTexture(Texture* tex, float sizeX = 50.f, float sizeY = 50.f);
		void SetTexture(std::wstring& name, float sizeX = 50.f, float sizeY = 50.f);

	public:
		template<typename Callback, typename... Args>
		void SetClickCallback(Callback&& cb, Args&&... args)
		{
			mClickCallback = std::bind(std::forward<Callback>(cb), std::forward<Args>(args)...);
		}

	private:
		std::function<void()>	mClickCallback;
		//std::function<void(void* data)> mClickCallback;
		bool					mClick;

		bool mbText;
		std::string mText;
		Texture* mTexture;
	};
}



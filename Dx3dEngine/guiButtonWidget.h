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
		void SetButtonText(const std::string& text) { mButtonText = text; }

		void SetTexture(Texture* tex);
		void SetTexture(const std::wstring& name);

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
		std::string mButtonText;
		Texture* mTexture;
	};
}



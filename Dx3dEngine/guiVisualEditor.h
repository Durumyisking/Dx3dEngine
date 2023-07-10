#pragma once
#include "guiWidget.h"

namespace gui
{
	class VisualEditor : public Widget
	{
	public:
		VisualEditor();
		~VisualEditor();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
		bool mbPadding;
		bool mbFullscreen;
		ImGuiDockNodeFlags mDockspace_flags;
	};
}
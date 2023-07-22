#pragma once
#include "guiWidget.h"
#include "GameObj.h"


namespace gui
{
	class GUIComponent : public Widget
	{
	public:
		GUIComponent(enums::eComponentType type);
		~GUIComponent();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(GameObj* target) { mTarget = target; }
		GameObj* GetTarget() const { return mTarget; }

	private:
		enums::eComponentType mType;
		GameObj* mTarget;
	};
}

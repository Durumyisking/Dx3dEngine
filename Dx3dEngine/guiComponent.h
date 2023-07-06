#pragma once
#include "guiWidget.h"
#include "GameObj.h"



namespace gui
{
	class Component : public Widget
	{
	public:
		Component(dru::enums::eComponentType type);
		~Component();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(dru::GameObj* target) { mTarget = target; }
		dru::GameObj* GetTarget() { return mTarget; }

	private:
		dru::enums::eComponentType mType;
		dru::GameObj* mTarget;
	};
}

#pragma once
#include "guiComponent.h"

namespace gui
{
	class GUITransform : public GUIComponent
	{
	public:
		GUITransform();
		~GUITransform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		math::Vector3 mPosisition;
		math::Vector3 mRotation;
		math::Vector3 mScale;
	};

}

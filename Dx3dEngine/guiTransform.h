#pragma once
#include "guiComponent.h"

namespace gui
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		dru::math::Vector3 mPosisition;
		dru::math::Vector3 mRotation;
		dru::math::Vector3 mScale;
	};

}

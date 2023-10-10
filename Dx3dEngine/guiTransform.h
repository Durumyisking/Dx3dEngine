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

		void InputFloatValues(math::Vector3& value);

		void IsPhysical(bool tf) { mbPhysical = tf; }

	private:
		bool mbPhysical;
		math::Vector3 mPosition;
		math::Vector3 mRotation;
		math::Vector3 mScale;
	};

}

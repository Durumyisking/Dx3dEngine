#pragma once
#include "guiComponent.h"

namespace gui
{
    class GUIPhysical :
        public GUIComponent
    {
	public:
		GUIPhysical();
		~GUIPhysical();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

	private:
		math::Vector3 mPosisition;
		math::Vector3 mRotation;
		math::Vector3 mScale;
    };
}


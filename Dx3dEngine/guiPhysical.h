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

		void Initialize();
		bool AddPhysical();
		void AddingPhysical(bool tf);

		void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

	private:
		bool mAddingPhysical;
		eActorType mActorType;
		eGeometryType mGeometryType;
		Vector3 mScale;
    };
}


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

		bool ChangePhysical();

		void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

	private:
		bool mbPhysical;
		bool mAddingPhysical;
		eActorType mActorType;
		eGeometryType mGeometryType;
		Vector3 mScale;
    };
}


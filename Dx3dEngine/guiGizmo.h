#pragma once
#include "guiWidget.h"
#include "ImGuizmo.h"
#include "guiEditorObject.h"
#include "SimpleMath.h"

namespace gui
{
    class Gizmo :
        public Widget
    {
	public:
		Gizmo();
		virtual ~Gizmo();

		void Initialize();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Close() override;


		void ClearTarget();
		void InitializeTargetGameObject();

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }

	private:
		int mGizmoCount;
		bool mbisOrthographic;
		float mOrthographicViewWidth;
		Camera* mCamera;
		GameObj* mTargetGameObject;
    };
}


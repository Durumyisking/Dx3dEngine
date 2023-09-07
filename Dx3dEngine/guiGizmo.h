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
		ImGuizmo::OPERATION GetGizmoOperation() { return mGizmoOperation; }
		void SetGizmoOperation(GameObj* target) { mGizmoOperation = ImGuizmo::TRANSLATE; }

	private:
		ImGuizmo::OPERATION mGizmoOperation;
		bool mbisOrthographic;
		float mOrthographicViewWidth;
		Camera* mCamera;
		GameObj* mTargetGameObject;
    };
}


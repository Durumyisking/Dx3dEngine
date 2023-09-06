#pragma once
#include "guiEditorObject.h"

namespace gui
{
    class Gizmo :
        public EditorObject
    {
	public:
		Gizmo();
		virtual ~Gizmo();

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

		void MoveTarget();
		EditorObject* GetSelectedBasis();

		void ClearTarget();
		void InitializeTargetGameObject();

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }

	private:
		GameObj* mTargetGameObject;

		EditorObject* mFoward;
		EditorObject* mUp;
		EditorObject* mRight;
    };
}


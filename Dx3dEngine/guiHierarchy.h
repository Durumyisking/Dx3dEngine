#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "GameObj.h"

class Scene;
namespace gui
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy();
		~Hierarchy();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitializeOutline(void* data);
		void InitializeScene();
		void AddGameObject(TreeWidget::Node* parent, GameObj* gameObject);

		GameObj* GetTargetObject() { return mTargetObject; }
		void SetTargetObject(GameObj* obj) { mTargetObject = obj; }
		void ForceReset() { mbForceReset = true; }

	private:
		bool mbForceReset;
		Scene* mCurrentScene;
		TreeWidget* mTreeWidget;
		GameObj* mTargetObject;
	};
}
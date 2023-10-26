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
		void DeleteGameObject();

		GameObj* GetTargetObject() { return mTargetObject; }
		void SetTargetObject(GameObj* obj) { mTargetObject = obj; }

	private:
		Scene* mCurrentScene;
		TreeWidget* mTreeWidget;
		GameObj* mTargetObject;
	};
}
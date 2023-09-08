#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "GameObj.h"

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

		void InitializeTreeWidget();

		GameObj* GetTargetObject() { return mTargetObject; }

	private:
		TreeWidget* mTreeWidget;
		GameObj* mTargetObject;
	};
}
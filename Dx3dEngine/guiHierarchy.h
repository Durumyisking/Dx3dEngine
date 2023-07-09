#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"

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

		void InitializeInspector(void* data);
		void InitializeScene();
		void AddGameObject(TreeWidget::Node* parent, class dru::GameObj* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}
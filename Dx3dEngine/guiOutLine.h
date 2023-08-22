#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "GameObj.h"


namespace gui
{
    class OutLine :
        public Widget
    {
	public:
		OutLine();
		~OutLine();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitializeInspector(void* data);
		void InitializeScene();
		void AddGameObject(TreeWidget::Node* parent, GameObj* gameObject);

	private:
		TreeWidget* mTreeWidget;
    };
}


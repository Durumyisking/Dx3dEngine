#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "GameObj.h"
#include "guiComponent.h"
#include "guiResource.h"
#include "guiGroupWidget.h"

namespace gui
{
	class GUIComponent;
	class GUIResource;

	class OutLiner : public Widget
	{
	public:
		OutLiner();
		~OutLiner();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ClearTarget();
		void InitializeTargetGameObject();

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }
		

		void AddComponent(std::string str);
		void toInspector(void* data);

	private:
		TreeWidget* mTreeWidget;
		GroupWidget* mGroupWidget;

		GameObj* mTargetGameObject;
		std::vector<Component*> mComponents;
	};
}


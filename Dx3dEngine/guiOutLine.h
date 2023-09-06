#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "GameObj.h"
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class GUIComponent;
	class GUIResource;
	class OutLine : public Widget
	{
	public:
		OutLine();
		~OutLine();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }
		Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

		bool GetComponentType(eComponentType type);

	private:
		GameObj* mTargetGameObject;
		Resource* mTargetResource;
		std::vector<GUIComponent*> mComponents;
		std::vector<GUIResource*> mResources;
	};
}


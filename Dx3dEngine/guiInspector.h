#pragma once
#include "guiWidget.h"
#include "GameObj.h"

namespace gui
{
	class Component;
	class Resource;
	class GameObj;
	class Inspector : public Widget
	{
	public:
		Inspector();
		~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		dru::GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(dru::GameObj* target) { mTargetGameObject = target; }
		dru::Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(dru::Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		dru::GameObj* mTargetGameObject;
		dru::Resource* mTargetResource;
		std::vector<gui::Component*> mComponents;
		std::vector<gui::Resource*> mResources;
	};
}
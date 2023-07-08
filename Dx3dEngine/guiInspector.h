#pragma once
#include "guiWidget.h"

namespace gui
{
	class Inspector : public Widget
	{
	public:
		Inspector();
		~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		class dru::GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(dru::GameObj* target) { mTargetGameObject = target; }
		class dru::Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(dru::Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		dru::GameObj* mTargetGameObject;
		dru::Resource* mTargetResource;
		std::vector<class Component*> mComponents;
		std::vector<class Resource*> mResources;
	};
}
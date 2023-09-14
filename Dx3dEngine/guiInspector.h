#pragma once
#include "guiWidget.h"
#include "GameObj.h"

namespace gui
{
	class GUIComponent;
	class GUIResource;
	class Inspector : public Widget
	{
	public:
		Inspector();
		~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }
		Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		GameObj* mTargetGameObject;
		Resource* mTargetResource;
		std::vector<gui::GUIComponent*> mComponents;
		std::vector<gui::GUIResource*> mResources;
	};
}
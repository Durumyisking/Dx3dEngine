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

		void ResetContent();

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

		GameObj* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObj* target) { mTargetGameObject = target; }
		Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(Resource* target) { mTargetResource = target; }

		bool GetComponentType(eComponentType type);

	private:
		template <typename T>
		void AddResources(TreeWidget::Node* rootNode, const char* name)
		{
			const std::vector<T*> resources
				= GETSINGLE(ResourceMgr)->Finds<T>();

			TreeWidget::Node* stemNode
				= mTreeWidget->AddNode(rootNode, name, 0, true);

			for (T* resource : resources)
			{
				std::string name(resource->GetName().begin(), resource->GetName().end());
				mTreeWidget->AddNode(stemNode, name, resource);
			}
		}

		void toInspector(void* data);

	private:
		TreeWidget* mTreeWidget;

		GameObj* mTargetGameObject;
		Resource* mTargetResource;

		std::vector<GUIComponent*> mComponents;
		std::vector<GUIResource*> mResources;
	};
}


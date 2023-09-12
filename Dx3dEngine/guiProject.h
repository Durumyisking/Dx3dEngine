#pragma once
#include "guiWidget.h"
#include "ResourceMgr.h"
#include "guiTreeWidget.h"
#include "guiGroupWidget.h"

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		~Project();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ResetContent();

	private:
		//template <typename T>
		//void AddResources(TreeWidget::Node* rootNode, const char* name)
		//{
		//	const std::vector<T*> resources
		//		= GETSINGLE(ResourceMgr)->Finds<T>();

		//	TreeWidget::Node* stemNode
		//		= mTreeWidget->AddNode(rootNode, name, 0, true);

		//	for (T* resource : resources)
		//	{
		//		std::string name(resource->GetName().begin(), resource->GetName().end());
		//		mTreeWidget->AddNode(stemNode, name, resource);
		//	}
		//}

		void FolderClickCallback(std::string path); 
		void OpenFolderCallback(); //���� ���� ��ư 
		void SelectFolder(std::string path);
		void toConsole(); //���� Ŭ�� ��


	private:
		GroupWidget* mFolders;
		
		std::string mTargetPath;
		std::vector<std::string> mTargetFolders;
	};
}
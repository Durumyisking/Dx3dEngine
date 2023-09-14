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
		void OpenFolderCallback(const std::string& path); //폴더 오픈 버튼 
		void SelectFolder(const std::string& path);
		void toConsole(); //폴더 클릭 시


	private:
		GroupWidget* mGroup;
		bool	mbTargetChanged;
		
		std::string mTargetPath;
		std::vector<std::string> mTargetFolders;
		std::vector<std::string> mTargetFiles;
	};
}
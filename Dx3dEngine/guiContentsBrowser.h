#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "ResourceMgr.h"

class Scene;
namespace gui
{
    class ContentsBrowser :
        public Widget
    {
	public:
		ContentsBrowser();
		~ContentsBrowser();

		bool SaveScene();
		bool LoadScene();

		void Initialize();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void TreeInitialize();
		void SetSceneType(UINT data);

	private:
		std::wstring mSceneSaveName;
		std::wstring mScenePath;
		std::wstring mCurrentSceneName;
		UINT mSceneSaveType;
		Scene* mScene;

		TreeWidget* mTreeWidget;
    };
}

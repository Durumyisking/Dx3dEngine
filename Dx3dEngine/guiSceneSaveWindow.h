#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "ResourceMgr.h"

class Scene;
namespace gui
{
    class SceneSaveWindow :
        public Widget
    {
	public:
		SceneSaveWindow();
		~SceneSaveWindow();

		bool SaveScene();
		bool LoadScene();

		void Initialize();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

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

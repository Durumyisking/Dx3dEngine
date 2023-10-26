#pragma once
#include "guiWidget.h"
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

		bool SaveLayerObjects();
		bool LoadLayerObjects();

		void Initialize();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetSceneType(UINT data);
		void SetLayerType(UINT data);

	private:
		std::wstring mSceneSaveName;
		std::wstring mScenePath;
		std::wstring mCurrentSceneName;
		UINT mSceneSaveType;
		Scene* mScene;

		UINT mLayerSaveType;

		class GroupWidget* mGroupWidget;
		class TreeWidget* mSceneTree;
		class TreeWidget* mLayerTree;
    };
}

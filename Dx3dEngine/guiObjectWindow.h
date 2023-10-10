#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "guiGroupWidget.h"
#include "GameObj.h"

namespace gui
{
    class ObjectWindow :
        public Widget
    {
	public:
		ObjectWindow();
		~ObjectWindow();

		void Initialize();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void CreateObject();
		void DeleteObject();
		void SetObjectLayerType(UINT num);
		bool AddObjectToScene(GameObj* obj, eLayerType type);

		void GetObjectCDO(const std::string& CDOTypeName);

	private:
		std::string mCDOName;
		std::string mInputText;
		eLayerType mInputLayer;

		GroupWidget* mGroupWidget;
    };
}

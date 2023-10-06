#pragma once
#include "guiWidget.h"

namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct Node : public GuiEntity
		{
			Node();
			~Node();

			void Update();

			void SetData(void* data) { mData = data; }
			void SetNumber(UINT num) { mNumber = num; }
			void SetStem(bool enable) { mbStem = enable; }

			void AddNode(Node* node);
			const std::vector<Node*>& GetChilds() { return mChilds; }

			void ChildsResize(UINT size) { mChilds.resize(size); }

			TreeWidget* mTreeWidget;
			void* mData;
			UINT mNumber;

			Node* mParent;
			std::vector<Node*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

		Node* AddNode(Node* parent, const std::string& name, void* data, bool stem = false, UINT num = -1);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(Node* node);

		void SetEvent(Widget* widget, std::function<void(void* data)> func)
		{
			mEventWidget = widget;
			mEvent = func;
		}

		void SetSimpleEvent(Widget* widget, std::function<void(UINT data)> func)
		{
			mEventWidget = widget;
			mSimpleEvent = func;
		}

		Node* GetRootNode() { return mRoot; }


	private:
		Node* mRoot;

		bool mbDummyRootUse;
		Node* mSelectedNode;

		Widget* mEventWidget;
		std::function<void(void* data)> mEvent;
		std::function<void(UINT data)> mSimpleEvent;
	};
}
#include "UIManager.h"


std::unordered_map<eUIType, UIBase*> UIManager::mUIs;
std::queue<eUIType> UIManager::mRequestUIQueue;
std::stack<UIBase*> UIManager::mUIBases;
UIBase* UIManager::mCurrentData = nullptr;

void UIManager::Initialize()
{
	//ui 메모리에 할당

	//UIBase* newUI = new UIBase(eUIType::HP);
	//mUIs.insert(std::make_pair(eUIType::HP, newUI));

	//newUI = new UIBase(eUIType::Coin);
	//mUIs.insert(std::make_pair(eUIType::Coin, newUI));

	//newUI = new UIBase(eUIType::Crosshair);
	//mUIs.insert(std::make_pair(eUIType::Crosshair, newUI));

}

void UIManager::OnLoad(eUIType type)
{
	//std::unordered_map<eUIType, UIBase*>::iterator iter = mUIs.find(type);

	//if (iter == mUIs.end())
	//{
	//	OnFail();
	//	return;
	//}
	//
	//OnComplete(iter->second);

}

void UIManager::Tick()
{
	if (mRequestUIQueue.size() > 0)
	{
		//UI 로드
		eUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI);
	}
}

void UIManager::Render()
{
	//std::stack<UIBase*> uiBases = mUIBases;
	//while (uiBases.empty())
	//{
	//	UIBase* uiBase = uiBases.top();
	//	if (uiBase != nullptr)
	//	{
	//		uiBase->Render();
	//	}
	//	uiBases.pop();
	//}
}

void UIManager::OnComplete(UIBase* addUI)
{
	if (addUI == nullptr)
		return;

	addUI->Initialize();
	addUI->Active();
	addUI->Update();

	if (addUI->GetIsFullScreen())
	{
		std::stack<UIBase*> uiBases = mUIBases;
		while (uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase->GetIsFullScreen())
			{
				uiBase->InActive();
			}
		}
	}

	mUIBases.push(addUI);
}

void UIManager::OnFail()
{
	mCurrentData = nullptr;
}

void UIManager::Push(eUIType type)
{
	mRequestUIQueue.push(type);
}

void UIManager::Pop(eUIType type)
{
	if (mUIBases.size() <= 0)
		return;

	std::stack<UIBase*> tempStack;


	UIBase* uiBase = nullptr;
	while (mUIBases.size() > 0)
	{
		uiBase = mUIBases.top();
		mUIBases.pop();

		// pop하는 ui가 전체화면 일경우에,
		//남은 ui 중에 전체화면인 가장 상단의 ui를 활성화

		if (uiBase->GetUIType() == type)
		{
			if (uiBase->GetIsFullScreen())
			{
				std::stack<UIBase*> uiBases = mUIBases;
				while (uiBases.empty())
				{
					UIBase* uiBase = uiBases.top();
					uiBases.pop();

					if (uiBase->GetIsFullScreen())
					{
						uiBase->Active();
						break;
					}
				}
			}
			uiBase->UIClear();
		}
		else
		{
			tempStack.push(uiBase);
		}
	}

	//스택 이터레이터가 없으므로
	//다시 넣어주는 작업을 하는것
	while (tempStack.size())
	{
		uiBase = tempStack.top();
		tempStack.pop();
		mUIBases.push(uiBase);
	}
}

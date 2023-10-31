#include "UIManager.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Material.h"

#include "LifeUI.h"
#include "Panal.h"

#include "DieCircleUIScript.h"

//std::unordered_map<eUIType, UIBase*> UIManager::mUIPanals;
//std::queue<eUIType> UIManager::mRequestUIQueue;
//std::stack<UIBase*> UIManager::mUIBases;
//UIBase* UIManager::mCurrentData = nullptr;
//currentUI UIManager::mCurrentUI = currentUI::End;
//int UIManager::mCount = 3;
//
//int UIManager::mCoin = 0;
//int UIManager::mCityCoin = 0;

UIManager::UIManager()
	:mUIPanals{}
	, mTitleUIPanal{}
	, mRequestUIQueue{}
	, mUIBases{}
	, mCurrentData(nullptr)
	, mCurrentUI(currentUI::End)
	, mCount(3)
	, mLife(3)
{
}

UIManager::~UIManager()
{

}

void UIManager::Initialize()
{
	mCurrentUI = currentUI::MainMenu;
}

void UIManager::OnLoad(eUIType type)
{
	std::unordered_map<eUIType, UIBase*>::iterator iter = mUIPanals.find(type);

	if (iter == mUIPanals.end())
	{
		OnFail();
		return;
	}

	OnComplete(iter->second);
}

void UIManager::Update()
{
	//switch (mCurrentUI)
	//{
	//case enums::currentUI::None:
	//	break;
	//case enums::currentUI::MainMenu:
	//	MainMenuUI();
	//	break;
	//case enums::currentUI::Play:
	//	PlayScene();
	//	PlayerHit();
	//	break;
	//case enums::currentUI::End:
	//	break;
	//default:
	//	break;
	//}
}

void UIManager::Render()
{

}

void UIManager::OnComplete(UIBase* addUI)
{
	if (addUI == nullptr)
		return;

	if (addUI->GetUIEnable())
		return;

	addUI->Activate();

	if (addUI->GetIsFullScreen())
	{
		std::stack<UIBase*> uiBases = mUIBases;
		while (uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase->GetIsFullScreen())
			{
				uiBase->InActivate();
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

		// pop�ϴ� ui�� ��üȭ�� �ϰ�쿡,
		//���� ui �߿� ��üȭ���� ���� ����� ui�� Ȱ��ȭ

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

	//���� ���ͷ����Ͱ� �����Ƿ�
	//�ٽ� �־��ִ� �۾��� �ϴ°�
	while (tempStack.size())
	{
		uiBase = tempStack.top();
		tempStack.pop();
		mUIBases.push(uiBase);
	}
}

void UIManager::PushPanal(eUIType type, UIBase* base)
{
	mUIPanals.insert(std::make_pair(type, base));
}

void UIManager::PushTitlePanal(eUIType type, UIBase* base)
{
	mTitleUIPanal.insert(std::make_pair(type, base));
}

void UIManager::UIActivate()
{

}

UIBase* UIManager::GetPanal(eUIType type)
{
	std::unordered_map<eUIType, UIBase*>::iterator iter = mUIPanals.find(type);

	if (iter == mUIPanals.end())
	{
		OnFail();
		return nullptr;
	}

	return iter->second;
}

UIBase* UIManager::GetTitlePanal(eUIType type)
{
	std::unordered_map<eUIType, UIBase*>::iterator iter = mTitleUIPanal.find(type);

	if (iter == mTitleUIPanal.end())
	{
		OnFail();
		return nullptr;
	}

	return iter->second;
}

void UIManager::MainMenuUI()
{

}

void UIManager::GetCoin()
{

}

void UIManager::GetLuna()
{

}

void UIManager::PlayScene()
{

}

void UIManager::PlayerDie()
{
	for (const auto& [key, value] : mUIPanals) 
	{
		if (key == eUIType::DiePanal)
			value->Activate();
		else
			value->InActivate();
	}

	std::unordered_map<eUIType, UIBase*>::iterator iter = mUIPanals.find(eUIType::DiePanal);

	iter->second->GetChilds()[0]->GetScript<DieCircleUIScript>()->DieEffectOn();
}

void UIManager::PlayerDieReset()
{
	for (const auto& [key, value] : mUIPanals)
	{
		if (key == eUIType::DiePanal)
		{
			value->InActivate();
		}
		else
		{
			value->Activate();
		}
	}
}
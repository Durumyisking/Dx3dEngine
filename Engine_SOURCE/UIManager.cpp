#include "UIManager.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Material.h"

#include "LifeUI.h"

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
	switch (mCurrentUI)
	{
	case enums::currentUI::None:
		break;
	case enums::currentUI::MainMenu:
		MainMenuUI();
		break;
	case enums::currentUI::Play:
		PlayScene();
		PlayerHit();
		break;
	case enums::currentUI::End:
		break;
	default:
		break;
	}
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

void UIManager::PushPanal(eUIType type, UIBase* base)
{
	mUIPanals.insert(std::make_pair(type, base));
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

void UIManager::MainMenuUI()
{
	if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::UP)))
	{
		if (GETSINGLE(UIManager)->mCount >= 3)
			return;

		(GETSINGLE(UIManager)->mCount++);

		for (size_t i = 0; i < (GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds().size()); i++)
		{
			(GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false));
		}

		(GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[GETSINGLE(UIManager)->mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4, 1.0f), true));
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)))
	{
		if (GETSINGLE(UIManager)->mCount <= 0)
			return;

		GETSINGLE(UIManager)->mCount--;

		for (size_t i = 0; i < GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds().size(); i++)
		{
			GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
		}

		GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[GETSINGLE(UIManager)->mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);
	}

	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		GETSINGLE(UIManager)->mCurrentUI = currentUI::Play;
		GETSINGLE(UIManager)->mCount = 3;
		return;
	}
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

void UIManager::PlayerHit()
{

}
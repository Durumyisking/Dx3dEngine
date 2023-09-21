#pragma once
#include "CommonInclude.h"
#include "UIBase.h"


class UIManager
{
	SINGLE(UIManager)


public:
	static void Initialize();
	static void OnLoad(eUIType type);
	static void Update();
	static void Render();
	static void OnComplete(UIBase* addUI);
	static void OnFail();

	static void Push(eUIType type);
	static void Pop(eUIType type);
	static void PushPanal(eUIType type, UIBase* base);
	static void UIActivate();
	static UIBase* GetPanal(eUIType type);


protected:

	static void MainMenuUI();

	static void GetCoin();
	static void GetLuna();
	static void PlayScene();
	static void PlayerHit();

private:

	static std::unordered_map<eUIType, UIBase*> mUIPanals;
	static std::queue<eUIType> mRequestUIQueue;
	static std::stack<UIBase*> mUIBases;
	static UIBase* mCurrentData;
	static currentUI mCurrentUI;
	static int mCount;

	static int mCoin;
	static int mCityCoin;
};
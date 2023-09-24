#pragma once
#include "CommonInclude.h"
#include "UIBase.h"


class UIManager
{
	SINGLE(UIManager)


public:
	 void Initialize();
	 void OnLoad(eUIType type);
	 void Update();
	 void Render();
	 void OnComplete(UIBase* addUI);
	 void OnFail();

	 void Push(eUIType type);
	 void Pop(eUIType type);
	 void PushPanal(eUIType type, UIBase* base);
	 void UIActivate();
	 UIBase* GetPanal(eUIType type);


protected:

	void MainMenuUI();

	void GetCoin();
	void GetLuna();
	void PlayScene();
	void PlayerHit();

private:

	std::unordered_map<eUIType, UIBase*> mUIPanals;
	std::queue<eUIType> mRequestUIQueue;
	std::stack<UIBase*> mUIBases;
	UIBase* mCurrentData;
	currentUI mCurrentUI;
	int mCount;

	int mCoin;
	int mCityCoin;
	int mLife;
};
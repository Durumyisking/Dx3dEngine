#pragma once
#include "UIScript.h"
#include "Panal.h"


class CoinUIScript : public UIScript
{
	enum class State
	{
		None,
		Idle,
		GetCoin,
		Die,
		LoseCoin,
		End,
	};


public:
	CoinUIScript();
	virtual ~CoinUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;


	void GetCoin();
	void LoseCoin();

	void Reset();
	void ActionToPlayerDied();

private:
	void ReturnToColor();
	void ReturnToFade();
	void CountLoseCoin();

	void ChangeColor();
	void ChangePos();

	void End();

	int mCoin;
	int mCount;
	int mLoseCoinCnt;


	bool mbCoin;
	bool mbDie;
	bool mbLoseCion;

	float mCurrentTime;


	State mState;
	Panal* mPanal;
};


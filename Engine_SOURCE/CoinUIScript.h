#pragma once
#include "UIScript.h"
#include "Panal.h"


class CoinUIScript : public UIScript
{
public:
	CoinUIScript();
	virtual ~CoinUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;


	void GetCoin();

private:
	int mCoin;
	bool mbCoin;
	float mCurrentTime;
	Panal* mPanal;
};


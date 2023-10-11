#pragma once
#include "Script.h"
#include "Panal.h"

class TitleBGMScript : public Script
{
	enum class State
	{
		None,
		MarioShout,
		StartBGM,
	};

public:
	TitleBGMScript();
	virtual ~TitleBGMScript();

	virtual void Initialize() override;
	virtual void Update() override;

	void SetStartSound() { mState = State::MarioShout; };

private:
	void MarioShout();
	void StartBGM();

	float mCurrentTime;
	Panal* mPanal;
	State mState;
};


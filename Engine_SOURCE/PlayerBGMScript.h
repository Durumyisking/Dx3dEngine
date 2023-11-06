#pragma once
#include "Script.h"

class Player;
class PlayerBGMScript : public Script
{
public: 
	enum class eSoundState : short
	{
		None,
		FootNote,
		Ha,
		End,
	};

public:
	PlayerBGMScript();
	virtual ~PlayerBGMScript();

	virtual void Initialize() override;
	virtual void Update() override;

	void SetSound(eSoundState state) { mState = state; }
	void SetStartSound() { mState = eSoundState::FootNote; };

private:
	void FootNote();
	void StartBGM();

	float mCurrentTime;
	Player* mPlayer;
	eSoundState mState;
};


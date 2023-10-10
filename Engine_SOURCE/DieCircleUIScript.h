#pragma once
#include "UIScript.h"
#include "Panal.h"

class DieCircleUIScript : public UIScript
{

	enum class State
	{
		None,
		Start,
		Idle,
		ReStart,
		End,
	};

public:
	DieCircleUIScript();
	virtual ~DieCircleUIScript();

	virtual void Initialize() override;
	virtual void Update() override;

	void DieEffectOn() { mState = State::Start; }

private:
	void Start();
	void Idle();
	void ReStart();
	void End();
	void Reset();

	Panal* mPanal;
	Transform* mTr;

	State mState;

	float mCurrentTime;

	Vector3 mOriginScale;
};
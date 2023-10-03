#pragma once
#include "UIScript.h"
#include "Panal.h"

class LifeUIScript : public UIScript
{
	enum class State
	{
		None,
		Hit,
		Move,
		Wait,
		Back,
	};


public:
	LifeUIScript();
	virtual ~LifeUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	void ChangeImage();
	void PlayerHit(float amplitude, float frequency);

	void Hit()
	{
		mState = State::Hit;
	}

	void ReactionHit();
	void MoveUI(float amplitude, float frequency);
	void BackUI();

private:
	int mLife;
	Panal* mPanal;

	float mShakeTime;
	float mAmplitude;
	float mFrequency;

	Vector3 mOriginPos;
	Vector3 mTargetPos;
	Vector4 mOriginColor;

	int mCount;

	State mState;
};


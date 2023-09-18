#pragma once
#include "DynamicObject.h"
#include "CaptureObj.h"

class BoneAnimator;
class Monster : public DynamicObject, public CaptureObj
{
public:
	enum class eMonsterState : short
	{
		Idle,
		Move,
		Jump,
		Attack,
		SpecialSituation,
		Hit,
		Groggy,
		Die,
	};

public:
	Monster();
	virtual ~Monster();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	virtual void CaptureEvent() = 0;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) {};
	virtual void stateInfoInitalize() = 0;
public:
	eMonsterState GetMonsterState() { return mMonsterState; }
	void SetMonsterState(eMonsterState monsterState);

	GETSET(const float, mRecognizeRadius, GetRecognizeRadius)
	GETSET(const bool, mFoundPlayer, IsFoundPlayer)

private:
	eMonsterState mMonsterState;

	float mRecognizeRadius;
	bool mFoundPlayer
};


#pragma once
#include "GameObj.h"
#include "CaptureObj.h"

class BoneAnimator;
class Monster : public GameObj, public CaptureObj
{
public:
	enum class eMonsterState : short
	{
		Idle,
		Move,
		Jump,
		Attack,
		SpecialCast,
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
public:
	GETSET(eMonsterState, mMonsterState, MonsterState)

private:
	eMonsterState mMonsterState;
};


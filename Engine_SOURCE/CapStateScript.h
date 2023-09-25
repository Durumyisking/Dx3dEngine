#pragma once
#include "Script.h"
#include "BoneAnimator.h"
#include "MarioCap.h"

#define Cap_SPPED 7000.f
#define Cap_JUMPFORCE 7000.f
#define Cap_MASS 1.f
#define Cap_WALK_VELOCITY 3.f
#define Cap_RUN_VELOCITY 10.f

class Cap;
class CapStateScript : public Script
{
public:
	CapStateScript();
	virtual ~CapStateScript();

	virtual void Update() override;
	virtual void Initialize() override;
public:
	virtual void Idle() final;
	virtual void Throw() final;
	virtual void Return() final;
	virtual void Capture() final;
	virtual void Die() final;

public:
	void Reset() { mbAnimationRunning = false; }

private:
	std::vector<std::function<void()>> mStateEventList;
	float mInitialForce = 33.0f; // 초기 힘
	float mForceIncrement = 0.05f; // 시간에 따른 힘의 증가량
	//float mMoveTime = 0.0f;

protected:
	MarioCap* mCap;
	BoneAnimator* mAnimator;
	bool mbAnimationRunning;
	float mTimer;
};


#pragma once
#include "Script.h"
#include "BoneAnimator.h"

class Player;
class PlayerStateScript : public Script
{
public:
	PlayerStateScript();
	virtual ~PlayerStateScript();

	virtual void Update() override;
	virtual void Initialize() override;
public:
	virtual void Idle() final;
	virtual void Move() final;
	virtual void Jump() final;
	virtual void Squat() final;
	virtual void SquatMove() final;
	virtual void Air() final;
	virtual void Wall() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void ThrowCap() final;
	virtual void Die() final;

public:
	void Reset() { mbAnimationRunning = false; }

private:
	std::vector<std::function<void()>> mStateEventList;
	float mInitialForce = 33.0f; // 초기 힘
	float mForceIncrement = 0.05f; // 시간에 따른 힘의 증가량
	//float mMoveTime = 0.0f;

protected:
	Player* mPlayer;
	bool mbAnimationRunning;
};


#pragma once
#include "Script.h"
#include "BoneAnimator.h"

#define PLAYER_SPPED 7000.f
#define PLAYER_JUMPFORCE 10000.f
#define PLAYER_MASS 1.f
#define PLAYER_WALK_VELOCITY 3.f
#define PLAYER_SQUATWALK_VELOCITY 1.f
#define PLAYER_RUN_VELOCITY 8.f

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
	virtual void Fall() final;
	virtual void Wall() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void ThrowCap() final;
	virtual void CatchCap() final;
	virtual void Capture() final;
	virtual void Die() final;

public:
	void Reset() { mbAnimationRunning = false; }

private:
	std::vector<std::function<void()>> mStateEventList;
	float mInitialForce = 7000.f; // 초기 힘
	float mForceIncrement = 20.f; // 시간에 따른 힘의 증가량
	//float mMoveTime = 0.0f;

protected:
	Player* mPlayer;
	BoneAnimator* mAnimator;
	bool mbAnimationRunning;
};


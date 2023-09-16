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
	virtual void Crouch() final;
	virtual void CrouchMove() final;
	virtual void Air() final;
	virtual void Wall() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void CapAction() final;
	virtual void Die() final;

public:
	void Reset() { mbAnimationRunning = false; }

private:
	std::vector<std::function<void()>> mStateEventList;

protected:
	Player* mPlayer;
	bool mbAnimationRunning;
};


#pragma once
#include "Script.h"
#include "BoneAnimator.h"

class Monster;
class MonsterStateScript : public Script
{
public:
	MonsterStateScript();
	virtual ~MonsterStateScript();

	virtual void Update() override;
	virtual void Initialize() override;
public:
	virtual void Idle();
	virtual void Move() {};
	virtual void Jump() {};
	virtual void Fall() {};
	virtual void Land() {};
	virtual void Turn() {};
	virtual void Chase() {};
	virtual void Attack() {};
	virtual void SpecialSituation() {};
	virtual void Hit() {};
	virtual void Groggy() {};
	virtual void Die() {};

public:
	void Reset() { mbAnimationRunning = false; }
	void MoveForward(float speed);
private:
	std::vector<std::function<void()>> mStateEventList;

protected:
	Monster* mMonster;
	GameObj* mPlayer;
	bool mbAnimationRunning;
	bool mbTurnLeft;
};


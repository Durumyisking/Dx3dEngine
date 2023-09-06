#pragma once
#include "Monster.h"

class Goomba : public Monster
{
public:
	Goomba();
	virtual ~Goomba();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	virtual void CaptureEvent() override;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) override;

private:

};


#pragma once
#include "Monster.h"

class Packun : public Monster
{
public:
	Packun();
	virtual ~Packun();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void CaptureEvent() final;


protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;

private:

};


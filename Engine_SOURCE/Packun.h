#pragma once
#include "Monster.h"

class GameObj;
class Packun : public Monster
{
public:
	Packun();
	virtual ~Packun();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void CaptureEvent() final;
	virtual void DivideEvnet() final;

	virtual void OnTriggerEnter(GameObj* gameObject) override;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;

private:
};


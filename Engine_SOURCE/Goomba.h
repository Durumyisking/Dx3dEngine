#pragma once
#include "Monster.h"

class Goomba : public Monster
{
public:
	Goomba();
	virtual ~Goomba();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void CaptureEvent() final;
	virtual void CaptureEnter(MarioCap* cap) final {};

	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerStay(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;


protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;

private:

};


#pragma once
#include "Monster.h"

class Goomba : public Monster
{
public:
	Goomba();
	Goomba(const Goomba& Obj);
	virtual ~Goomba();

	virtual Goomba* Clone() const;
	virtual void Save(FILE* File) final;
	virtual void Load(FILE* File) final;

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void CaptureEvent() final;

	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerStay(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;


protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;

private:

};


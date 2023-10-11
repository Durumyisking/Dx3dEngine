#pragma once
#include "Monster.h"

class GameObj;
class Packun : public Monster
{
public:
	Packun();
	Packun(const Packun& Obj);
	virtual ~Packun();

	virtual Packun* Clone() const;
	virtual void Save(FILE* File) final;
	virtual void Load(FILE* File) final;

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void CaptureEvent() final;
	virtual void CaptureEnter(MarioCap* cap) final;
	virtual void DivideEvent() final;

	virtual void OnTriggerEnter(GameObj* gameObject) override;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;

private:
};


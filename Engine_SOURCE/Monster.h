#pragma once
#include "DynamicObject.h"
<<<<<<< HEAD
class Monster :
    public DynamicObject
{
public:
=======
#include "CaptureObj.h"

class BoneAnimator;
class Monster : public DynamicObject, public CaptureObj
{
public:
	enum class eMonsterState : short
	{
		Idle,
		Move,
		Jump,
		Attack,
		SpecialSituation,
		Hit,
		Groggy,
		Die,
	};

public:
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
	Monster();
	virtual ~Monster();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
<<<<<<< HEAD
	virtual void Render() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

=======

	virtual void CaptureEvent() = 0;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) {};
	virtual void stateInfoInitalize() = 0;
public:
	eMonsterState GetMonsterState() { return mMonsterState; }
	void SetMonsterState(eMonsterState monsterState);

private:
	eMonsterState mMonsterState;
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
};


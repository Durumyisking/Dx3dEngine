#pragma once
#include "DynamicObject.h"
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
		Fall,
		Land,
		Turn,
		Chase,
		Attack,
		SpecialSituation,
		Hit,
		Groggy,
		Die,
	};

public:

	Monster();
	Monster(const Monster& Obj);
	virtual ~Monster();

	virtual Monster* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;


	virtual void CaptureEvent() override {};
	virtual void CaptureEnter(class MarioCap* cap) override ;

	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject)  override;
	virtual void OnTriggerExit(GameObj* gameObject)  override;

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) {};

	virtual void stateInfoInitalize() {};

	virtual void captureEnterModelOperation() {};

public:
	eMonsterState GetMonsterState() { return mMonsterState; }
	void SetMonsterState(eMonsterState monsterState);

	GETSET(const float, mRecognizeRadius, GetRecognizeRadius)
	GETSET(const bool, mFoundPlayer, IsFoundPlayer)


private:
	eMonsterState mMonsterState;

	float mRecognizeRadius;
	bool mFoundPlayer;
};


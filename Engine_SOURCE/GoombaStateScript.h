#pragma once
#include "MonsterStateScript.h"

//class BoneAnimator	;
//class PhysXRigidBody;
//class PhysicalMovement;
//class Transform		;
#define GOOMBA_SPPED 100.f
#define GOOMBA_MASS 1.f
#define GOOMBA_WALK_VELOCITY 2.5f
#define GOOMBA_RUN_VELOCITY 5.f
class GoombaStateScript : public MonsterStateScript
{
public:
	GoombaStateScript();
	virtual ~GoombaStateScript();

	virtual void Initialize() final;
	virtual void Update() final;

public:
	virtual void Idle() final;
	virtual void Move() final;
	virtual void Jump() final;
	virtual void SpecialSituation() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void Die() final;
private:
	BoneAnimator*		 mAnimator ;
	PhysXRigidBody*		 mRigidbody; 
	PhysicalMovement*	 mMovement ;
	Transform*			 mTransform;
};


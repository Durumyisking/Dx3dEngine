#pragma once
#include "MonsterStateScript.h"

//class BoneAnimator	;
//class PhysXRigidBody;
//class PhysicalMovement;
//class Transform		;
#define GOOMBA_SPPED 5000.f
#define GOOMBA_JUMPFORCE 7000.f
#define GOOMBA_MASS 1.f
#define GOOMBA_WALK_VELOCITY 3.f
#define GOOMBA_RUN_VELOCITY 5.f

class Model;
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
	virtual void Fall()  final;
	virtual void Land()  final;
	virtual void Turn()  final;
	virtual void Chase() final;

	virtual void SpecialSituation() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void Die() final;


private:
	void setHalfCloseEyeModel();
	void setOpenEyeModel();
	void rotateByKey();

private:
	BoneAnimator*		 mAnimator ;
	PhysXRigidBody*		 mRigidbody; 
	PhysicalMovement*	 mMovement ;
	Transform*			 mTransform;

	Model*				 mModel;

	
};


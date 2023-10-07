#pragma once
#include "Entity.h"


using namespace enums;

class GameObj;
class Transform;
class Camera;
class RigidBody;
class PhysXRigidBody;
class PhysicalMovement;
class Physical;
class PhysXCollider;
class Animator;
class BoneAnimator;
class MeshRenderer;
class SpriteRenderer;
class ParticleSystem;
class Light;

class Component : public DruEntity
{
public:
	Component(eComponentType type);
	virtual ~Component();

	virtual void Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void FixedUpdate()	= 0;
	virtual void PrevRender() {};
	virtual void Render()		= 0;

	eComponentType GetOrder() { return mType; }

	void SetOwner(GameObj* owner) { mOwner = owner; }
	GameObj* GetOwner() { return mOwner; }

	bool IsSwitchOn() const { return mbSwitch; }
	void SetSwitchState(bool flag) { mbSwitch = flag; }

	template <typename T>
	T* GetOwnerType()
	{
		GameObj* owner = GetOwner();
		return dynamic_cast<T*>(owner);

	}

	math::Vector3 GetOwnerPos() const; // component script에서 코드 사용 편하게 하기위함
	math::Vector3 GetOwnerWorldPos() const; 

	Transform* GetTransform() ;
	Camera* GetCamera() ;
	RigidBody* GetRigidBody() ;
	PhysXRigidBody* GetPhysXRigidBody();
	PhysicalMovement* GetMovement() ;
	Physical* GetPhysical() ;
	PhysXCollider* GetPhysXCollider();
	Animator* GetAnimator() ;
	BoneAnimator* GetBoneAnimator() ;
	MeshRenderer* GetMeshRenderer() ;
	SpriteRenderer* GetSpriteRenderer() ;
	ParticleSystem* GetParticle() ;
	Light* GetLight() ;

private:
	const eComponentType mType; // 타입이 바뀌면 안되기 때문
	GameObj* mOwner;
	bool mbSwitch;
};




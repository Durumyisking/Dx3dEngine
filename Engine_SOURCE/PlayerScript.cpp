#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "PxFilter.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"


namespace dru
{
	PlayerScript::PlayerScript()
		: mTransform(nullptr)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();

	}
	void PlayerScript::Update()
	{

	}
	void PlayerScript::FixedUpdate()
	{
		Vector3 pos = {};
		Vector3 velocity = {};
		if (KEY_DOWN(A))
		{
			mTransform->AddRotationX(50.f * DT);
		}
		if (KEY_DOWN(S))
		{
			mTransform->AddRotationY(50.f * DT);
		}
		if (KEY_DOWN(D))
		{
			mTransform->AddRotationZ(50.f * DT);
		}


		if (KEY_DOWN(LEFT))
		{
			velocity = GetOwner()->GetComponent<Transform>()->Right() * - 5.f;
			GetOwner()->GetComponent<PhysXRigidBody>()->SetVelocity(AXIS::X, velocity.x);
		}
		if (KEY_DOWN(RIGHT))
		{
			velocity = GetOwner()->GetComponent<Transform>()->Right() * 5.f;
			GetOwner()->GetComponent<PhysXRigidBody>()->SetVelocity(AXIS::X, velocity.x);
		}
		if (KEY_DOWN(UP))
		{
			velocity = GetOwner()->GetComponent<Transform>()->Up() * 5.f;
			GetOwner()->GetComponent<PhysXRigidBody>()->SetVelocity(AXIS::Y, velocity.y);
		}
	}
	void PlayerScript::Render()
	{
	}
	void PlayerScript::fontRender()
	{
	}
	void PlayerScript::OnCollisionEnter(Collider2D* oppo)
	{
	}
	void PlayerScript::OnCollision(Collider2D* oppo)
	{
	}
	void PlayerScript::OnCollisionExit(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTriggerEnter(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTrigger(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTriggerExit(Collider2D* oppo)
	{
	}
}
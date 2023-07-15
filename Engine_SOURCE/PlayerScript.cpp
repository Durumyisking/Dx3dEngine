#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "PxFilter.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"
#include "TimerMgr.h"

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

		Timer* timer = new Timer(3.f);
		timer->SetEvent() = [this] 
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Up() * 10000.f)), PxForceMode::Enum::eFORCE); 
		};
		GETSINGLE(TimerMgr)->AddTimer(timer);

		GetOwner()->GetComponent<PhysXRigidBody>()->SetAngularMaxVelocityForDynamic(10.f);
		GetOwner()->GetComponent<PhysXRigidBody>()->SetLinearMaxVelocityForDynamic(10.f);
	}
	void PlayerScript::Update()
	{

	}
	void PlayerScript::FixedUpdate()
	{
		Vector3 pos = {};
		Vector3 velocity = {};
		if (KEY_DOWN(I))
		{
			mTransform->AddRotationX(50.f * DT);
		}
		if (KEY_DOWN(O))
		{
			mTransform->AddRotationY(50.f * DT);
		}
		if (KEY_DOWN(P))
		{
			mTransform->AddRotationZ(50.f * DT);
		}


		if (KEY_DOWN(LEFT))
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Right() * -10.f)), PxForceMode::Enum::eFORCE);
		}
		if (KEY_DOWN(RIGHT))
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Right() * 10.f)), PxForceMode::Enum::eFORCE);
		}
		if (KEY_DOWN(UP))
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Forward() * 10.f)), PxForceMode::Enum::eFORCE);
		}
		if (KEY_DOWN(DOWN))
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Forward() * -10.f)), PxForceMode::Enum::eFORCE);
		}
		if (KEY_DOWN(SPACE))
		{
			GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic(convert::Vector3ToPxVec3((mTransform->Up() * 10000.f)), PxForceMode::Enum::eFORCE);
		}
		if (KEY_DOWN(R))
		{
			mTransform->SetPhysicalPosition(Vector3(-5.f, 0.f, 5.f));
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
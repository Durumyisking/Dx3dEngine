#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "PxFilter.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"
#include "TimerMgr.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"

//temp
#include "SceneMgr.h"
#include "Scene.h"


PlayerScript::PlayerScript()
	: mTransform(nullptr)
	, mPhyXRigidBody(nullptr)
{
}
PlayerScript::~PlayerScript()
{
}
void PlayerScript::Initialize()
{
	mTransform = GetOwner()->GetComponent<Transform>();
	mPhyXRigidBody = GetOwner()->GetComponent<PhysXRigidBody>();

	mPhyXRigidBody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	mPhyXRigidBody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);

	//mPhyXRigidBody->SetAngularMaxVelocityForDynamic(10.f);
	//mPhyXRigidBody->SetLinearMaxVelocityForDynamic(10.f);
}
void PlayerScript::Update()
{
	Vector3 pos = {};
	Vector3 velocity = {};
	Transform* camTransform = renderer::mainCamera->GetOwner()->GetComponent<Transform>();
	Vector3 camUp = camTransform->Up();
	Vector3 camForward = camTransform->Forward();
	Vector3 camRight = camTransform->Right();

	camRight.y = 0.f;
	camForward.y = 0.f;

	if (KEY_DOWN(I))
	{
		mPhyXRigidBody->AddTorqueXForDynamic(100.f * DT);
	}
	if (KEY_DOWN(O))
	{
		mPhyXRigidBody->AddTorqueYForDynamic(100.f * DT);
	}
	if (KEY_DOWN(P))
	{
		mPhyXRigidBody->AddTorqueZForDynamic(100.f * DT);
	}

	// 카메라의 forward와 player의 forward내적
	float cDotp = camForward.Dot(mTransform->Forward());
	float cDotp_degree = toDegree(cDotp);



	if (KEY_DOWN(LEFT))
	{
		mPhyXRigidBody->AddForce(-camRight * 7000.f * DT);
	}
	if (KEY_DOWN(RIGHT))
	{
		mPhyXRigidBody->AddForce(camRight * 7000.f * DT);
	}
	if (KEY_DOWN(UP))
	{
		mPhyXRigidBody->AddForce(camForward * 7000.f * DT);
	}
	if (KEY_DOWN(DOWN))
	{
		mPhyXRigidBody->AddForce(-camForward * 7000.f * DT);
	}
	if (KEY_TAP(SPACE))
	{
		GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic((Vector3::Up * 5000.f * DT), PxForceMode::Enum::eIMPULSE);
	}

}
void PlayerScript::FixedUpdate()
{

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


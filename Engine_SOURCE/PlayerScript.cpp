#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "PxFilter.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"
#include "TimerMgr.h"
#include "PhysXCollider.h"

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

	mPhyXRigidBody->SetAngularMaxVelocityForDynamic(10.f);
	mPhyXRigidBody->SetLinearMaxVelocityForDynamic(10.f);

}
void PlayerScript::Update()
{

}
void PlayerScript::FixedUpdate()
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


	if (KEY_DOWN(LEFT))
	{
		mPhyXRigidBody->AddForceForDynamic((camRight * -1000.f * DT), PxForceMode::Enum::eFORCE);
	}
	if (KEY_DOWN(RIGHT))
	{
		mPhyXRigidBody->AddForceForDynamic((camRight * 1000.f * DT), PxForceMode::Enum::eFORCE);
	}
	if (KEY_DOWN(UP))
	{
		mPhyXRigidBody->AddForceForDynamic((camForward * 1000.f * DT), PxForceMode::Enum::eFORCE);
	}
	if (KEY_DOWN(DOWN))
	{
		mPhyXRigidBody->AddForceForDynamic((camForward * -1000.f * DT), PxForceMode::Enum::eFORCE);
	}
	if (KEY_TAP(SPACE))
	{
		GetOwner()->GetComponent<PhysXRigidBody>()->AddForceForDynamic((mTransform->Up() * 500000.f * DT), PxForceMode::Enum::eFORCE);
	}
	if (KEY_DOWN(T))
	{
		Vector3 origin = mTransform->GetWorldPosition() + (mTransform->GetScaleZ() * Vector3(0.f, 0.f, 1.f));

		const auto& gameObjects = GETSINGLE(SceneMgr)->GetActiveScene()->GetLayer(eLayerType::Objects).GetGameObjects();
		for (const auto& pGameObject : gameObjects)
		{
			if (GetOwner()->GetComponent<PhysXCollider>()->Raycast(origin, Vector3(0.f, 0.f, 1.f), pGameObject, 5.f))
			{
				int i = 0;
			}
			else
			{
				int i = 0;
			}
		}	
	}
	if (KEY_DOWN(Z))
	{
		const auto& gameObjects = GETSINGLE(SceneMgr)->GetActiveScene()->GetLayer(eLayerType::Objects).GetGameObjects();
		for (const auto& pGameObject : gameObjects)
			renderer::mainCamera->Raycast(renderer::mainCamera->GetOwnerWorldPos(), Vector3(0.f, 0.f, 1.f), pGameObject);
	}

	if (KEY_DOWN(R))
	{
		mTransform->SetPhysicalPosition(Vector3(-5.f, 5.f, 5.f));
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


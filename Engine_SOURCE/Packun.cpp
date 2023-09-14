#include "Packun.h"
#include "Object.h"
#include "InputMgr.h"
#include "BoneAnimator.h"
#include "Model.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"
#include "PackunStateScript.h"
#include "PhysXRigidBody.h"
#include "Physical.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"

#include "PackunPostionBall.h"

Packun::Packun()
	: Monster()
{
	OnCapture();
}

Packun::~Packun()
{
}

void Packun::Initialize()
{
	// Add MeshRenderer
	MeshRenderer* mesh = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Packun");
	if (model)
	{
		MeshRenderer* test = GetComponent<MeshRenderer>();
		test->SetModel(model, model->GetMaterial(0));


		//// 오프
		//model->MeshRenderSwtich(L"Head2__BodyMT-mesh", false);
		//model->MeshRenderSwtich(L"Head2__HeadMT-mesh", false);
		//model->MeshRenderSwtich(L"mustache__HairMT-mesh", false);

		//// 온ㄴ
		//model->MeshRenderSwtich(L"Head3__BodyMT-mesh");
		//model->MeshRenderSwtich(L"Head3__HeadMT-mesh");
	}

	PackunStateScript* packunState = AddComponent<PackunStateScript>(eComponentType::Script);
	packunState->Initialize();

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

	// 초기화
	Monster::Initialize();

	// state초기화
	stateInfoInitalize();
}

void Packun::Update()
{
	Monster::Update();
}

void Packun::FixedUpdate()
{
	Monster::FixedUpdate();
}

void Packun::CaptureEvent()
{
	// 캡처 이벤트 구현부
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// 키 입력 이벤트 처리하는 람다식
	std::function<void(eKeyState, eKeyCode, eMonsterState)> stateEvent =
		[&]
		(eKeyState keyState, eKeyCode curPress, eMonsterState nextState) ->void
		{
			if (able)
				return;
			if (keyEvent[static_cast<UINT>(keyState)](curPress))
			{
				SetMonsterState(nextState);
				able = true;
			}
		};

	// 이동
	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::RIGHT, eMonsterState::Move);

	// 공격
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Attack);
}

void Packun::stateInfoInitalize()
{
	//Idle
	// 현재는 대기상태에서 못가는상태가 없다

	// Jump
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Groggy));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Die));

	//Attack
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::SpecialSituation));

	//SpecialCast
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Move));

	// Hit
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::SpecialSituation));

	// Groggy
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::SpecialSituation));

	// Die
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Idle));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Groggy));
}

void Packun::boneAnimatorInit(BoneAnimator* animator)
{
	animator->LoadAnimations(L"..//Resources/Packun/Animation");
	AnimationClip* cilp = nullptr;

	// 어택 동작 연계
	{
		cilp = animator->GetAnimationClip(L"Attack");
		if (cilp)
			cilp->SetCompleateEvent([this]() 
				{
					SetMonsterState(Monster::eMonsterState::Idle);
					PackunPostionBall* packunball = object::LateInstantiate<PackunPostionBall>(eLayerType::Objects);
					PhysXRigidBody* rigidbody = packunball->GetComponent<PhysXRigidBody>();
					if (rigidbody)
					{
						rigidbody->AddForceForDynamic(Vector3(50000.f, 50000.f, 0.f), PxForceMode::eFORCE);
					}

					Vector3 rotation = GetComponent<Transform>()->GetRotation();
					packunball->GetComponent<Transform>()->SetPhysicalPosition(Vector3(50.f, 50.f, 0.0f));
					packunball->GetComponent<Transform>()->SetPhysicalRotation(rotation);
				});
	}

	// 피격 동작 연계
	{
		cilp = animator->GetAnimationClip(L"AttackHit");
		if (cilp)
			cilp->SetCompleateEvent([this]()
				{
					SetMonsterState(Monster::eMonsterState::Idle);
				});
	}


}

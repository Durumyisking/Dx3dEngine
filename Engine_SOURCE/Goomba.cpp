#include "Goomba.h"
#include "InputMgr.h"
#include "BoneAnimator.h"
#include "Model.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"
#include "GoombaStateScript.h"
#include "PhysXRigidBody.h"
#include "Physical.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"


Goomba::Goomba()
	: Monster()
{
	OnCapture();
	SetName(L"Goomba");
}

Goomba::~Goomba()
{
}

void Goomba::Initialize()
{
	SetGetRecognizeRadius(15.f);	


	// Add MeshRenderer
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"goomba");
	assert(model);

	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	model->MeshRenderSwtich(L"EyeClose__BodyMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", false);
	model->MeshRenderSwtich(L"Mustache__HairMT-mesh", false);
	model->MeshRenderSwtich(L"PressModel__BodyMT-mesh", false);

	model->SetVariableMaterialsByKey(0, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(1, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(2, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(7, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(8, L"goombaEye0Material");
	model->SetVariableMaterialsByKey(9, L"goombaEye0Material");

	//Phsical^
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	assert(physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f));

	// Rigidbody
	assert(AddComponent<PhysXRigidBody>(eComponentType::RigidBody));
	//GetComponent<PhysXRigidBody>()->RemoveGravity();

	// MoveMent
	assert(AddComponent<PhysXCollider>(eComponentType::Collider));
	
	// Collider
	assert(AddComponent<PhysicalMovement>(eComponentType::Movement));

	// Script
	assert(AddComponent<GoombaStateScript>(eComponentType::Script));

	// 상태 info 초기화	
	stateInfoInitalize();

	// 초기화
	Monster::Initialize();

}

void Goomba::Update()
{
	Monster::Update();

}

void Goomba::FixedUpdate()
{
	Monster::FixedUpdate();
}

void Goomba::CaptureEvent()
{
	// 캡처 이벤트 구현부
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	// getkeytap의 첫번째 인자인 this는 inputmgr 싱글톤 포인터를 고정으로 사용, 두번째 인자는 유동적으로 사용하겠다.
	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// 키 입력 이벤트 처리하는 람다식
	std::function<void(eKeyState, eKeyCode, eMonsterState)> stateEvent = 
		[&]
		(eKeyState keyState,eKeyCode curPress, eMonsterState nextState) ->void
		{
			if (able)
				return;
			if(keyEvent[static_cast<UINT>(keyState)](curPress))
			{
				SetMonsterState(nextState);
				able = true;
			}
		};

	// 이동
	//stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	//stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	//stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	//stateEvent(eKeyState::DOWN,eKeyCode::RIGHT, eMonsterState::Move);

	//// 점프
	//able = false;
	stateEvent(eKeyState::TAP, eKeyCode::Y, eMonsterState::Jump);

	// 특수
	//able = false;
	//stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::SpecialCast);
}

void Goomba::OnCollisionEnter(GameObj* gameObject)
{
}

void Goomba::OnTriggerEnter(GameObj* gameObject)
{
	if (eLayerType::Platforms == gameObject->GetLayerType())

	{
		if (eMonsterState::Fall == GetMonsterState())
		{
			SetMonsterState(Monster::eMonsterState::Land);
		}
		GetPhysXRigidBody()->SetAirOff();
	}
}

void Goomba::OnTriggerStay(GameObj* gameObject)
{
}

void Goomba::OnTriggerExit(GameObj* gameObject)
{
	if (eLayerType::Platforms == gameObject->GetLayerType())
	{
		GetPhysXRigidBody()->ApplyGravity();
		GetPhysXRigidBody()->SetAirOn();
	}
}

void Goomba::boneAnimatorInit(BoneAnimator* animator)
{
	animator->LoadAnimations(L"..//Resources/goomba/Animation");

	AnimationClip* cilp = animator->GetAnimationClip(L"Attack");
	if (cilp)
		cilp->SetCompleteEvent([this]() {SetMonsterState(Monster::eMonsterState::Idle); });
}

void Goomba::stateInfoInitalize()
{
	//Idle
	// 현재는 대기상태에서 못가는상태가 없다

	// Move
	InsertLockState(static_cast<UINT>(eMonsterState::Move), static_cast<UINT>(eMonsterState::Move));

	// Jump
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Groggy));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Die));

	//Attack
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Jump));
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
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Hit));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Groggy));
}

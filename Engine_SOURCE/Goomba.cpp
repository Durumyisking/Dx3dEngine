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
}

Goomba::~Goomba()
{

}

void Goomba::Initialize()
{
	// Add MeshRenderer
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"goomba");
	if(model)
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	model->SetVariableMaterialsByKey(0, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(1, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(2, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(7, L"goombaBodyMaterial");
	model->SetVariableMaterialsByKey(8, L"goombaEye0Material");
	model->SetVariableMaterialsByKey(9, L"goombaEye0Material");


	GoombaStateScript* goombaState = AddComponent<GoombaStateScript>(eComponentType::Script);
	//goombaState->Initialize();

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigidbody->Initialize();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider);
	AddComponent<PhysicalMovement>(eComponentType::Movement);

	// 초기화
	Monster::Initialize();

	// 
	stateInfoInitalize();
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
	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	stateEvent(eKeyState::DOWN,eKeyCode::RIGHT, eMonsterState::Move);

	// 점프
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Jump);

	// 특수
	//able = false;
	//stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::SpecialCast);
}

void Goomba::boneAnimatorInit(BoneAnimator* animator)
{
	animator->LoadAnimations(L"..//Resources/goomba/Animation");

	AnimationClip* cilp = animator->GetAnimationClip(L"Attack");
	if (cilp)
		cilp->SetCompleateEvent([this]() {SetMonsterState(Monster::eMonsterState::Idle); });
}

void Goomba::stateInfoInitalize()
{
	//Idle
	// 현재는 대기상태에서 못가는상태가 없다

	// Move
	InsertLockState(static_cast<UINT>(eMonsterState::Move), static_cast<UINT>(eMonsterState::Move));

	// Jump
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

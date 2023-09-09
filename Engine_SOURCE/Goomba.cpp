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
	//OnCapture();
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

	GoombaStateScript* goombaState = AddComponent<GoombaStateScript>(eComponentType::Script);
	goombaState->Initialize();

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();

	// MoveMent
	//AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

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
	mStateInfo[static_cast<UINT>(eMonsterState::Idle)].mLockState;

	// Jump
	mStateInfo[static_cast<UINT>(eMonsterState::Jump)].mLockState.insert(static_cast<UINT>(eMonsterState::Move));
	mStateInfo[static_cast<UINT>(eMonsterState::Jump)].mLockState.insert(static_cast<UINT>(eMonsterState::SpecialSituation));
	mStateInfo[static_cast<UINT>(eMonsterState::Jump)].mLockState.insert(static_cast<UINT>(eMonsterState::Groggy));
	mStateInfo[static_cast<UINT>(eMonsterState::Jump)].mLockState.insert(static_cast<UINT>(eMonsterState::Die));

	//Attack
	mStateInfo[static_cast<UINT>(eMonsterState::Attack)].mLockState.insert(static_cast<UINT>(eMonsterState::Jump));
	mStateInfo[static_cast<UINT>(eMonsterState::Attack)].mLockState.insert(static_cast<UINT>(eMonsterState::SpecialSituation));

	//SpecialCast
	mStateInfo[static_cast<UINT>(eMonsterState::SpecialSituation)].mLockState.insert(static_cast<UINT>(eMonsterState::Attack));

	// Hit
	mStateInfo[static_cast<UINT>(eMonsterState::Hit)].mLockState.insert(static_cast<UINT>(eMonsterState::Move));
	mStateInfo[static_cast<UINT>(eMonsterState::Hit)].mLockState.insert(static_cast<UINT>(eMonsterState::Jump));
	mStateInfo[static_cast<UINT>(eMonsterState::Hit)].mLockState.insert(static_cast<UINT>(eMonsterState::Attack));
	mStateInfo[static_cast<UINT>(eMonsterState::Hit)].mLockState.insert(static_cast<UINT>(eMonsterState::SpecialSituation));

	// Groggy
	mStateInfo[static_cast<UINT>(eMonsterState::Groggy)].mLockState.insert(static_cast<UINT>(eMonsterState::Move));
	mStateInfo[static_cast<UINT>(eMonsterState::Groggy)].mLockState.insert(static_cast<UINT>(eMonsterState::Jump));
	mStateInfo[static_cast<UINT>(eMonsterState::Groggy)].mLockState.insert(static_cast<UINT>(eMonsterState::Attack));
	mStateInfo[static_cast<UINT>(eMonsterState::Groggy)].mLockState.insert(static_cast<UINT>(eMonsterState::SpecialSituation));

	// Die
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::Idle));
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::Move));
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::Jump));
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::Attack));
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::SpecialSituation));
	mStateInfo[static_cast<UINT>(eMonsterState::Die)].mLockState.insert(static_cast<UINT>(eMonsterState::Groggy));

}

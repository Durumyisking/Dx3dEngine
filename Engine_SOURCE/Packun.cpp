#include "Packun.h"
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
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Packun");
	if (model)
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	PackunStateScript* packunState = AddComponent<PackunStateScript>(eComponentType::Script);
	packunState->Initialize();

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();

	// MoveMent
	//AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

	// �ʱ�ȭ
	Monster::Initialize();

	// state�ʱ�ȭ
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
	// ĸó �̺�Ʈ ������
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// Ű �Է� �̺�Ʈ ó���ϴ� ���ٽ�
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

	// �̵�
	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::RIGHT, eMonsterState::Move);

	// ����
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Attack);
}

void Packun::stateInfoInitalize()
{
	//Idle
	// ����� �����¿��� �����»��°� ����

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

	// ���� ���� ����
	{
		cilp = animator->GetAnimationClip(L"Attack");
		if (cilp)
			cilp->SetCompleateEvent([this]() 
				{
					SetMonsterState(Monster::eMonsterState::Idle);
				});
	}

	// �ǰ� ���� ����
	{
		cilp = animator->GetAnimationClip(L"AttackHit");
		if (cilp)
			cilp->SetCompleateEvent([this]()
				{
					SetMonsterState(Monster::eMonsterState::Idle);
				});
	}


}
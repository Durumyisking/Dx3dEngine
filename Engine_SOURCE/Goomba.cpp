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

	MeshRenderer* mr =  GetComponent<MeshRenderer>();
	mr->SetModel(model);
	model->MeshRenderSwtich(L"EyeClose__BodyMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", false);
	model->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", false);
	model->MeshRenderSwtich(L"Mustache__HairMT-mesh", false);
	model->MeshRenderSwtich(L"PressModel__BodyMT-mesh", false);

	// body
	mr->SetMaterialByKey(L"goombaBodyMaterial", 0);

	// eyebrow
	mr->SetMaterialByKey(L"goombaBodyMaterial", 1);
	mr->SetMaterialByKey(L"goombaBodyMaterial", 2);

	// close
	mr->SetMaterialByKey(L"goombaBodyMaterial", 3);

	// halfclose
	mr->SetMaterialByKey(L"goombaBodyMaterial", 4);
	mr->SetMaterialByKey(L"goombaEye0Material", 5);
	mr->SetMaterialByKey(L"goombaEye0Material", 6);

	// open
	mr->SetMaterialByKey(L"goombaBodyMaterial", 7);
	mr->SetMaterialByKey(L"goombaEye0Material", 8);
	mr->SetMaterialByKey(L"goombaEye0Material", 9);

	// mustatch

	// press
	mr->SetMaterialByKey(L"goombaBodyMaterial", 11);

	//Phsical^
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	assert(physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 1.f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.5f, 1.f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	// Rigidbody
	assert(AddComponent<PhysXRigidBody>(eComponentType::RigidBody));

	// MoveMent
	assert(AddComponent<PhysXCollider>(eComponentType::Collider));
	
	// Collider
	assert(AddComponent<PhysicalMovement>(eComponentType::Movement));

	// Script
	assert(AddComponent<GoombaStateScript>(eComponentType::Script));

	// ���� info �ʱ�ȭ	
	stateInfoInitalize();

	// �ʱ�ȭ
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
	// ĸó �̺�Ʈ ������
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	// getkeytap�� ù��° ������ this�� inputmgr �̱��� �����͸� �������� ���, �ι�° ���ڴ� ���������� ����ϰڴ�.
	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// Ű �Է� �̺�Ʈ ó���ϴ� ���ٽ�
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

	// �̵�
	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	stateEvent(eKeyState::DOWN,eKeyCode::RIGHT, eMonsterState::Move);

	//// ����
	//able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Jump);

	// Ư��
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

	if (eLayerType::Player == gameObject->GetLayerType())
	{
		Vector3 goombaToPlayer = gameObject->GetWorldPos() - GetWorldPos();
		goombaToPlayer.Normalize();
		Vector3 goombaUpVector = GetTransform()->WorldUp();

		float cosTheta = goombaToPlayer.Dot(goombaUpVector);
		if (cosTheta > 0.95f)
		{
			Model* model = GetMeshRenderer()->GetModel();
			model->AllMeshRenderSwtichOff();
			model->MeshRenderSwtich(L"PressModel__BodyMT-mesh");
			GetBoneAnimator()->Play(L"PressDown");
			SetMonsterState(Monster::eMonsterState::Die);
		}
	}

	Monster::OnTriggerEnter(gameObject);
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
	// ����� �����¿��� �����»��°� ����

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

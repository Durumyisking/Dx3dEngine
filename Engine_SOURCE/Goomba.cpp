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
	, mGoombaLayerIdx(0)
	, mLowerLayerGoombas{}
	, mModel(nullptr)
{
	SetName(L"Goomba");
	mObjectTypeName = "Goomba";
}

Goomba::Goomba(const Goomba& Obj)
	: Monster(Obj)
{
	OnCapture();
	SetName(L"Goomba");
}

Goomba::~Goomba()
{
}

Goomba* Goomba::Clone() const
{
	return new Goomba(*this);
}

void Goomba::Save(FILE* File)
{
	Monster::Save(File);
}

void Goomba::Load(FILE* File)
{
	Monster::Load(File);
}

void Goomba::Initialize()
{
	SetGetRecognizeRadius(15.f);	


	// Add MeshRenderer
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	// SetModel
	mModel = GETSINGLE(ResourceMgr)->Find<Model>(L"goomba");
	assert(mModel);

	MeshRenderer* mr =  GetComponent<MeshRenderer>();
	mr->SetModel(mModel);
	mModel->MeshRenderSwtich(L"EyeClose__BodyMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", false);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", false);
	mModel->MeshRenderSwtich(L"Mustache__HairMT-mesh", false);
	mModel->MeshRenderSwtich(L"PressModel__BodyMT-mesh", false);

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
	mr->SetMaterialByKey(L"MarioMustacheMaterial", 10);

	// press
	mr->SetMaterialByKey(L"goombaBodyMaterial", 11);

	//Phsical^
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	assert(physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

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

	// ����� ��� ���� ��
	if (mLowerLayerGoombas.size())
	{
		// Ʈ��������
		PxTransform bottomTr = mLowerLayerGoombas[0]->GetTransform()->GetPxTransform();
		PxTransform tr = GetTransform()->GetPxTransform();

		tr.p.x = bottomTr.p.x;
		tr.p.z = bottomTr.p.z;
		tr.p.y = bottomTr.p.y + 1.5f * mGoombaLayerIdx;
		GetTransform()->SetPhysicalPosition(convert::PxVec3ToVector3(tr.p));

		tr.q = bottomTr.q; // ȸ���� �����ϰ�
		GetTransform()->SetPhysicalRotation(tr.q);
	}

	// �ִϸ����� ������Ʈ ���� ���ƾ���
	if (mLowerLayerGoombas.size())
	{
		// �ִϸ��̼� �����ϰ�
		std::wstring animName = GetBoneAnimator()->PlayAnimationName();
		std::wstring lowerGoombaAnimName = mLowerLayerGoombas[0]->GetBoneAnimator()->PlayAnimationName();
		bool loop = mLowerLayerGoombas[0]->GetBoneAnimator()->GetLoop();

		if (animName != lowerGoombaAnimName)
		{
			GetBoneAnimator()->Play(lowerGoombaAnimName, loop);
			GetBoneAnimator()->GetPlayAnimation()->SetSkeletonData(mLowerLayerGoombas[0]->GetBoneAnimator()->GetPlayAnimation()->GetSkeletonData());
			GetBoneAnimator()->GetPlayAnimation()->SetCurIndex(mLowerLayerGoombas[0]->GetBoneAnimator()->GetPlayAnimation()->GetCurIndex());
			GetBoneAnimator()->GetPlayAnimation()->SetDuration(mLowerLayerGoombas[0]->GetBoneAnimator()->GetPlayAnimation()->GetDuration());
		}
	}


}

void Goomba::FixedUpdate()
{
	Monster::FixedUpdate();
}

void Goomba::PrevRender()
{
	modelSetting();

	Monster::PrevRender();
}

void Goomba::Render()
{
	modelSetting();

	Monster::Render();
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
	stateEvent(eKeyState::DOWN, eKeyCode::W, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::S, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::A, eMonsterState::Move);
	stateEvent(eKeyState::DOWN,eKeyCode::D, eMonsterState::Move);

	//// ����
	//able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Move);

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
		if (GetPhysXRigidBody()->IsOnAir())
		{
			SetMonsterState(Monster::eMonsterState::Land);
			GetPhysXRigidBody()->SetAirOff();
		}
	}

	if (eLayerType::Player == gameObject->GetLayerType())
	{
		if (Calculate_RelativeDirection_ByCosTheta(gameObject) > 0.95f)
		{
			Model* model = GetMeshRenderer()->GetModel();
			//model->AllMeshRenderSwtichOff();
			//model->MeshRenderSwtich(L"PressModel__BodyMT-mesh");
			GetBoneAnimator()->Play(L"PressDown");
			SetMonsterState(Monster::eMonsterState::Die);
		}
	}

	if (eLayerType::Cap == gameObject->GetLayerType())
	{
	}

	if (eLayerType::Monster == gameObject->GetLayerType())
	{
		if (L"Goomba" == gameObject->GetName())
		{
			Goomba* goomba = dynamic_cast<Goomba*>(gameObject);

			Vector3 pent = GetPhysXCollider()->ComputePenetration(gameObject);
			if (pent.y == 0.f && GetPhysXRigidBody()->GetVelocity() != Vector3::Zero)
			{
				GetPhysXRigidBody()->SetVelocity(AXIS::XZ, Vector3(0.f, 0.f, 0.f));
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pent);
			}

			// if �ɸ��� ������ (���� Ÿ���϶��� �� �� ���ٸ� �������)
			if (mLowerLayerGoombas.empty())
			{
				if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.9f)
				{
					// �Ʒ����� ���� ����
					std::vector<Goomba*> vec = goomba->GetGoombaLayer();
					mLowerLayerGoombas.clear();
					mLowerLayerGoombas.assign(vec.begin(), vec.end());

					// �Ʒ����� pushback
					mLowerLayerGoombas.emplace_back(goomba);
					++mGoombaLayerIdx;

					GetPhysXRigidBody()->SetSwitchState(false);
					GetPhysXRigidBody()->RemoveGravity();
					GetPhysXRigidBody()->SetVelocity(Vector3::Zero);
					GetScript<GoombaStateScript>()->SetSwitchState(false);
					SetMonsterState(eMonsterState::Idle);

					OffCapture();

					mLowerLayerGoombas[0]->OnCapture();
					mLowerLayerGoombas[0]->CopyCaptureData(dynamic_cast<CaptureObj*>(this));
				}
			}

			//// �Ʒ�����
			//if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.95f)
			//{

			//}
		}
	}


	Monster::OnTriggerEnter(gameObject);
}

void Goomba::OnTriggerPersist(GameObj* gameObject)
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

void Goomba::captureEnterModelOperation()
{
	// todo : ���� ĸó������ �� ó���ϴ� �Լ�

}

void Goomba::setHalfCloseEyeModel()
{
	assert(mModel);

	mModel->AllMeshRenderSwtichOff();
	mModel->MeshRenderSwtich(L"Body__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeBrow__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeHalfClose__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeLMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeHalfClose__EyeRMT-mesh", true);
}

void Goomba::setOpenEyeModel()
{
	assert(mModel);

	mModel->AllMeshRenderSwtichOff();
	mModel->MeshRenderSwtich(L"Body__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeBrow__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeOpen__BodyMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeLMT-mesh", true);
	mModel->MeshRenderSwtich(L"EyeOpen__EyeRMT-mesh", true);
}

void Goomba::setPressedModel()
{
	assert(mModel);

	mModel->AllMeshRenderSwtichOff();
	mModel->MeshRenderSwtich(L"PressModel__BodyMT-mesh", true);
}

void Goomba::setCapturedModel()
{
	if (GetCap())
	{
		mModel->AllMeshRenderSwtichOff();
		mModel->MeshRenderSwtich(L"Body__BodyMT-mesh", true);
		mModel->MeshRenderSwtich(L"EyeBrowCap__BodyMT-mesh", true);
		mModel->MeshRenderSwtich(L"EyeOpen__BodyMT-mesh", true);
		mModel->MeshRenderSwtich(L"EyeOpen__EyeLMT-mesh", true);
		mModel->MeshRenderSwtich(L"EyeOpen__EyeRMT-mesh", true);
		mModel->MeshRenderSwtich(L"Mustache__HairMT-mesh", true);
	}

}

void Goomba::modelSetting()
{
	switch (mMonsterState)
	{
	case Monster::eMonsterState::Idle:
	case Monster::eMonsterState::Move:
	case Monster::eMonsterState::Jump:
	case Monster::eMonsterState::Fall:
	case Monster::eMonsterState::Land:
	case Monster::eMonsterState::Turn:
		setOpenEyeModel();
		break;
	case Monster::eMonsterState::Chase:
	case Monster::eMonsterState::Attack:
		setHalfCloseEyeModel();
		break;
	case Monster::eMonsterState::Die:
		setPressedModel();
		break;
	case Monster::eMonsterState::SpecialSituation:
	case Monster::eMonsterState::Hit:
	case Monster::eMonsterState::Groggy:
	default:
		break;
	}
	
	setCapturedModel();
}

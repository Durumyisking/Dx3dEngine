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

#include "MarioCap.h"
#include "TimeMgr.h"
#include "Player.h"
#include "EffectObject.h"
#include "Object.h"

Goomba::Goomba()
	: Monster()
	, mGoombaLayerIdx(0)
	, mLowerLayerGoombas{}
	, mModel(nullptr)
	, mTopGoomba(this)
	, mRigidbody(nullptr)
{
	SetName(L"Goomba");
	mObjectTypeName = "Goomba";
}


Goomba::Goomba(const Goomba& Obj)
	: Monster(Obj)
	, mGoombaLayerIdx(0)
	, mLowerLayerGoombas{}
	, mModel(nullptr)
	, mTopGoomba(this)
	, mRigidbody(nullptr)
{
	//OnCapture();
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
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

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
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 0.3f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.5f, 0.3f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	// Rigidbody
	mRigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	assert(mRigidbody);

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider);
	// Collider
	AddComponent<PhysicalMovement>(eComponentType::Movement);
	// Script
	AddComponent<GoombaStateScript>(eComponentType::Script);

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
	if (KEY_TAP(R))
	{
		if (0 == mLowerLayerGoombas.size())
		{
			mMonsterState = eMonsterState::Idle;
			GetPhysXRigidBody()->SetVelocity(Vector3::Zero);
			GetPhysXRigidBody()->ApplyGravity();
			GetPhysXRigidBody()->SetAirOff();
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
	stateEvent(eKeyState::DOWN, eKeyCode::W, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::S, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::A, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::D, eMonsterState::Move);

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
		if (mRigidbody->IsOnAir())
		{
			SetMonsterState(Monster::eMonsterState::Land);
			mRigidbody->SetAirOff();
		}
	}

	if (eLayerType::Player == gameObject->GetLayerType())
	{
		if (Player::ePlayerState::Capture != dynamic_cast<Player*>(gameObject)->GetPlayerState())
		{
			if (Calculate_RelativeDirection_ByCosTheta(gameObject) > 0.95f)
			{
				Model* model = GetMeshRenderer()->GetModel();
				GetBoneAnimator()->Play(L"PressDown");
				SetMonsterState(Monster::eMonsterState::Die);
			}
		}
	}

	if (eLayerType::Objects == gameObject->GetLayerType())
	{
		Vector3 pentDir = GetPhysXCollider()->ComputePenetration_Direction(gameObject);
		Vector3 pentDirDepth = GetPhysXCollider()->ComputePenetration(gameObject);
		if (KEY_NONE(W) && KEY_NONE(S) && KEY_NONE(A) && KEY_NONE(D))
		{
			SetMonsterState(Monster::eMonsterState::Idle);
		}

		if (!(pentDir == Vector3::Zero))
		{
			if (mRigidbody->IsOnAir())
			{
				mRigidbody->SetAirOff();
				mRigidbody->RemoveGravity();
				if (KEY_DOWN(W) || KEY_DOWN(S) || KEY_DOWN(A) || KEY_DOWN(D))
				{
					SetMonsterState(Monster::eMonsterState::Move);
				}
			}

			//SetPlayerState(Player::ePlayerState::Idle);
			pentDirDepth.x = 0.f;
			pentDirDepth.z = 0.f;
			GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pentDirDepth);

		}
	}

	if (eLayerType::Monster == gameObject->GetLayerType())
	{
		if (L"Goomba" == gameObject->GetName())
		{
			Goomba* goomba = dynamic_cast<Goomba*>(gameObject);

			// ���ٳ��� �浹�� �о�� ����
			Vector3 pent = GetPhysXCollider()->ComputePenetration(gameObject);
			if (pent.y == 0.f)
			{
				mRigidbody->SetVelocity(AXIS::XZ, Vector3(0.f, 0.f, 0.f));
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pent);
			}

	
			// ���� �״´�. ( �켱 ĸó���϶��� ������� )
			// �� 1�� - ��� 1��
			// ���۱�(ĸó)�� �Ʒ����ٿ�, cap�� �� ���ٿ� ����
			// �� 2�� - ��� 1��
			// ���۱��� �Ʒ����ٷ� �̵���Ű�� cap�� �״��
			// ���� �ݺ�

			// �� 2�� - ��� 2��
			// ����� �� �� ���ٸ� ã�Ƽ� ���۱��� �ش�.

			// dvide�� cap owner�� ���� �̷������ �Ѵ�.
			
			// ���� 1�� ���ٸ�
			if (mLowerLayerGoombas.empty())
			{
				// if �ɸ��� ���� ������
				if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.8f)
				{
					// �Ʒ����� ���� ����
					std::vector<Goomba*> vec = goomba->GetGoombaLayer();
					mLowerLayerGoombas.clear();
					mLowerLayerGoombas.assign(vec.begin(), vec.end());

					// �Ʒ����� pushback
					mLowerLayerGoombas.emplace_back(goomba);
					++mGoombaLayerIdx;

					mRigidbody->SetSwitchState(false);
					mRigidbody->RemoveGravity();
					mRigidbody->SetVelocity(Vector3::Zero);
					GetScript<GoombaStateScript>()->SetSwitchState(false);
					SetMonsterState(eMonsterState::Idle);

					goomba->SetTopGoomba(this->GetTopGoomba());

					if (IsCapture())
					{
						OffCapture();
						mLowerLayerGoombas[0]->OnCapture();
						mLowerLayerGoombas[0]->CopyCaptureData(dynamic_cast<CaptureObj*>(this), false);
					}
				}
			}

			//// �Ʒ�����
			//if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.95f)
			//{

			//}
		}
	}

	if (eLayerType::Cap == gameObject->GetLayerType())
	{
		MarioCap* cap = dynamic_cast<MarioCap*>(gameObject);
		// top goomba�� ĸó enter
		if (mTopGoomba)
		{
			mTopGoomba->CaptureEnter(cap);
			mTopGoomba->OffCapture();
		}
		else
		{
			CaptureEnter(cap);
		}
		SetPlayer(dynamic_cast<Player*>(cap->GetOwner()));
		if (mLowerLayerGoombas.size())
		{
			mLowerLayerGoombas[0]->OnCapture();
			mLowerLayerGoombas[0]->SetMonsterState(eMonsterState::Idle);
		}
		else
		{
			SetMonsterState(eMonsterState::Idle);
		}
		// capture���� bottom ���ٿ��� �����
	}
	GameObj::OnTriggerEnter(gameObject);
}

void Goomba::OnTriggerPersist(GameObj* gameObject)
{
	if (eLayerType::Objects == gameObject->GetLayerType())
	{
		Vector3 pentDir = GetPhysXCollider()->ComputePenetration_Direction(gameObject);
		Vector3 pentDirDepth = GetPhysXCollider()->ComputePenetration(gameObject);

		if (KEY_NONE(W) && KEY_NONE(S) && KEY_NONE(A) && KEY_NONE(D))
		{
			SetMonsterState(Monster::eMonsterState::Idle); 
		}
		if (!(pentDir == Vector3::Zero))
		{
			if (mRigidbody->IsOnAir())
			{
				mRigidbody->SetAirOff();
				mRigidbody->RemoveGravity();

				// ��ǲ �������� idle��
				if (KEY_DOWN(W) || KEY_DOWN(S) || KEY_DOWN(A) || KEY_DOWN(D))
				{
					SetMonsterState(Monster::eMonsterState::Move);
				}
			}
			//SetPlayerState(Player::ePlayerState::Idle);

			pentDirDepth.x = 0.f;
			pentDirDepth.z = 0.f;
			GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pentDirDepth);

		}
	}

}

void Goomba::OnTriggerExit(GameObj* gameObject)
{
	if (eLayerType::Platforms == gameObject->GetLayerType())
	{
		mRigidbody->ApplyGravity();
		mRigidbody->SetAirOn();
	}

	if (eLayerType::Objects == gameObject->GetLayerType())
	{
		if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.65f)
		{
			if (!mRigidbody->IsOnAir())
			{
				mRigidbody->SetAirOn();
				mRigidbody->ApplyGravity();
				SetMonsterState(Monster::eMonsterState::Fall);
			}
		}
	}
	GameObj::OnTriggerExit(gameObject);
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

void Goomba::TrampleEffect()
{
	// ���� ����Ʈ
	float angle = -180.f;
	std::vector<EffectObject*> objects;
	objects.push_back(object::LateInstantiate<EffectObject>(eLayerType::Objects));
	objects.push_back(object::LateInstantiate<EffectObject>(eLayerType::Objects));
	objects.push_back(object::LateInstantiate<EffectObject>(eLayerType::Objects));
	objects.push_back(object::LateInstantiate<EffectObject>(eLayerType::Objects));
	objects.push_back(object::LateInstantiate<EffectObject>(eLayerType::Objects));


	for (auto i : objects)
	{
		i->Initialize();
	}

	Transform* tr = GetComponent<Transform>();
	Vector3 position = tr->GetPhysicalPosition();

	for (auto i : objects)
	{
		i->GetComponent<Transform>()->SetPhysicalPosition(position);
		i->GetComponent<Transform>()->SetPhysicalRotation(math::Vector3(0.0f,0.0f, angle));

		PhysXRigidBody* rigidbody = i->GetComponent<PhysXRigidBody>();

		if (rigidbody)
		{
			//rigidbody->SetMaxVelocity_Y(5.f);
			rigidbody->ApplyGravity();
			rigidbody->SetAirOn();

			//Vector3 force = -i->GetComponent<Transform>()->Forward() * 350.f * DT;
			Vector3 force = -Vector3(0.0f, angle ,0.0f) * DT;

			rigidbody->AddForce(force);
		
		}
		angle += 72.f;
	}


}
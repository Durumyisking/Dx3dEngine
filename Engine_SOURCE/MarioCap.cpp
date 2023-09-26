#include "MarioCap.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "BoneAnimator.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"
#include "PhysXCollider.h"
#include "Transform.h"
#include "CapStateScript.h"
#include "Monster.h"
#include "GenericAnimator.h"
#include "TimeMgr.h"

#include "Player.h"

MarioCap::MarioCap()
	: DynamicObject()
	, mCapState(eCapState::Idle)
	, mOwner(nullptr)
{
	SetLayerType(eLayerType::Player);
}

MarioCap::~MarioCap()
{
}

void MarioCap::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	AddComponent<CapStateScript>(eComponentType::Script);
	//AddComponent<Transform>(eComponentType::Transform);
	//AddComponent<Transform>(eComponentType::Transform);

	//model initialize
	SetPos(Vector3(0.f, 0.f, 0.f));

	// TEST
	SetPos(Vector3(-20.f, 0.f, 0.f));

	SetScale(Vector3(0.01f, 0.01f, 0.01f));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioCap");
	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	BoneAnimator* animator = GetComponent<BoneAnimator>(); 
	boneAnimatorInit(animator);

	AddComponent<GenericAnimator>(eComponentType::GenericAnimator);

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Sphere, Vector3(0.5f, 1.0f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	physical->RemoveActorToPxScene();

	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();
	rigidbody->RemoveGravity();
	rigidbody->SetAirOff();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider);
	AddComponent<PhysicalMovement>(eComponentType::Movement);


	//model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
	//model->SetParentTargetBone(L"Armature_Cap");
	//model->SetTargetBone(L"Armature_nw4f_root");
	//model->SetOffsetRotation(Vector3(0.0f, 1.570796f, -1.570796f * 2));

	mStateInfo.resize(static_cast<int>(eCapState::Die) + 1);
	stateInfoInitalize();
	Physicalinit();



	DynamicObject::Initialize();
}

void MarioCap::Update()
{
	DynamicObject::Update();
}

void MarioCap::FixedUpdate()
{
	if (mOwner == nullptr)
		return;

	if (GetCapState() == eCapState::Idle)
	{
		Transform* tr = GetTransform();
		Transform* playerTr = mOwner->GetTransform();

		tr->SetPhysicalPosition(playerTr->GetPhysicalPosition());
		tr->SetPhysicalRotation(playerTr->GetPhysicalRotation());
	}

	DynamicObject::FixedUpdate();
}

void MarioCap::Render()
{
	DynamicObject::Render();
}

void MarioCap::FontRender()
{
}

void MarioCap::Physicalinit()
{
	////Phsical
	//Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	//physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));
	//physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Sphere, Vector3(0.5f, 1.f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	//// Rigidbody
	//PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigidbody->Initialize();
	//rigidbody->RemoveGravity();
	//rigidbody->SetAirOff();

	//// MoveMent
	//AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	//AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

}

void MarioCap::OnCollisionEnter(GameObj* gameObject)
{

}

void MarioCap::OnTriggerEnter(GameObj* gameObject)
{
	if (gameObject->GetLayerType() == eLayerType::Monster)
	{
		Monster* monster = dynamic_cast<Monster*>(gameObject);
		Transform* tr = monster->GetComponent<Transform>();
		PhysXRigidBody* rigidbody = GetComponent<PhysXRigidBody>();

		Vector3 position = tr->GetPhysicalPosition();
		Vector3 rotation = tr->GetRotation();

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioCap");

		//GetComponent<Transform>()->SetPhysicalPosition(position + Vector3(0.f, 1.f, 0.f));
		GetComponent<Transform>()->SetPhysicalRotation(rotation);
		rigidbody->SetMaxVelocity(0.0f);

		GetComponent<BoneAnimator>()->Play(L"Capture", false);
		SetCapState(MarioCap::eCapState::Capture);
	}
}

void MarioCap::OnTriggerExit(GameObj* gameObject)
{
}

void MarioCap::boneAnimatorInit(BoneAnimator* animator)
{
	AnimationClip* cilp = nullptr;
	//animator->LoadAnimations(L"..//Resources/MarioCap/Animation");

	animator->CreateAnimation(L"ThrowCap", L"..//..//Resources/MarioCap/Animation/ThrowCap.smd", 0.05f);
	animator->CreateAnimation(L"Capture", L"..//..//Resources/MarioCap/Animation/Capture.smd", 120);
	animator->CreateAnimation(L"FlyingStart", L"..//..//Resources/MarioCap/Animation/FlyingStart.smd", 0.05f);
	animator->CreateAnimation(L"FlyingWait", L"..//..//Resources/MarioCap/Animation/FlyingWait.smd", 0.05f);
	animator->CreateAnimation(L"CatchCap", L"..//..//Resources/MarioCap/Animation/CatchCap.smd", 0.05f);
	//animator->Play(L"Capture");

	//���� ������ flyStart
	{
		cilp = animator->GetAnimationClip(L"ThrowCap");
		if (cilp)
			cilp->SetCompleteEvent([animator, this]()
		{
			animator->Play(L"Capture");
			FlyStart();
		});
	}

	//flyStart�� fly
	{
		/*cilp = animator->GetAnimationClip(L"FlyingStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"FlyingWait");
		});*/
	}

	//CatchCap �� idle��
	{
		cilp = animator->GetAnimationClip(L"CatchCap");
		if (cilp)
			cilp->SetCompleteEvent([this]()
		{
			SetCapState(MarioCap::eCapState::Idle);
		});
	}
}

void MarioCap::SetCapState(eCapState capState)
{

	std::set<UINT>::iterator iter;
	iter = mStateInfo[static_cast<UINT>(mCapState)].mLockState.find(static_cast<UINT>(capState));
	if (iter != mStateInfo[static_cast<UINT>(mCapState)].mLockState.end())
		return;

	mCapState = capState;

	CapStateScript* script = GetScript<CapStateScript>();
	if (script)
		script->Reset();

}

void MarioCap::stateInfoInitalize()
{
	
	//InsertLockState(static_cast<UINT>(eCapState::Capture), static_cast<UINT>(eCapState::Throw));
}


void MarioCap::FlyStart()
{
	// ������ �������̴� �ִϸ����� ����
	GenericAnimator* animator = GetComponent<GenericAnimator>();
	if (animator->IsRunning())
		animator->Stop();

	// �÷��̾��� ���� �����ǰ� Player forWard �� ������
	Transform* tr = GetTransform();
	Vector3 pos = tr->GetPhysicalPosition();
	Vector3 playerforward = mOwner->GetTransform()->WorldForward();

	AnimatorParam param;
	// ����Ÿ��
	param.AnimType = eAnimType::Linear;

	// ������ ���� Value ��
	param.StartValue = 0.f;
	param.EndValue= 15.f;

	// ����ð�
	param.DurationTime = 0.5;

	// ���� �Լ� std::function<void(float)>
	param.DurationFunc = [this, tr, pos, playerforward](float inCurValue)
	{
		tr->SetPhysicalPosition(pos + (-playerforward * inCurValue));
	};

	// ������ ȣ��Ǵ� �Լ�
	param.CompleteFunc = [this](float inCurValue)
	{
		FlyEnd();
	};

	// �̺�Ʈ ����
	animator->Start(param);
}

void MarioCap::FlyEnd()
{
	// ������ �������̴� �ִϸ����� ����
	GenericAnimator* animator = GetComponent<GenericAnimator>();
	if (animator->IsRunning())
		animator->Stop();

	// PlayerTransform
	Transform* playerTr = mOwner == nullptr ? nullptr : mOwner->GetTransform();
	// ThisTransform
	Transform* myTr = GetTransform();

	AnimatorParam param;
	// ����Ÿ��
	param.AnimType = eAnimType::Linear;

	param.StartValue = 1.f;
	param.EndValue = 30.f;

	// ����ð�
	param.DurationTime = 10.f;

	// ���� �Լ� std::function<void(float)>
	param.DurationFunc = [this, animator, playerTr, myTr](float inCurValue)
	{
		Vector3 playerPos = playerTr->GetPhysicalPosition();
		Vector3 myPos = myTr->GetPhysicalPosition();

		// �÷��̾���� ���͸�����
		Vector3 Dirction = playerPos - myPos; 

		// �÷��̾�� ������ �Ÿ��� ����������� ����
		if (Dirction.Length() <= 0.5f)
			animator->Stop(true);

		// ���⺤�� ����ȭ
		Dirction.Normalize();

		// �ش�������� �̵�
		float speed = 20.f;

		// inCurValue �� ���ϹǷν� �ð��� ��������� ���ƿ��� �ӵ��� ������
		myTr->SetPhysicalPosition(myPos + (Dirction * speed * inCurValue * DT));

	};

	// ������ ȣ��Ǵ� �Լ�
	param.CompleteFunc = [this, animator, playerTr, myTr](float inCurValue)
	{
		myTr->SetPhysicalPosition(playerTr->GetPhysicalPosition());
		myTr->SetPhysicalRotation(playerTr->GetPhysicalRotation());

		Player* player = dynamic_cast<Player*>(mOwner);
		if (player)
		{
			player->SetPlayerState(Player::ePlayerState::CatchCap);
		}

		SetCapState(eCapState::Return);
		// �������� ���ڸ� ������
		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
		//model->MeshRenderSwtich(L"Cap__CapMT-mesh", true);

		GetPhysical()->RemoveActorToPxScene();
	};

	// �̺�Ʈ ����
	animator->Start(param);
}
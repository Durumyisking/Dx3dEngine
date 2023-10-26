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

	mObjectTypeName = "MarioCap";
	SetLayerType(eLayerType::Cap);
}

MarioCap::MarioCap(const MarioCap& Obj)
	: DynamicObject(Obj)
	, mCapState(Obj.mCapState)
	, mOwner(Obj.mOwner)
{
	SetLayerType(eLayerType::Player);
}

MarioCap::~MarioCap()
{
}

MarioCap* MarioCap::Clone() const
{
	return new MarioCap(*this);
}

void MarioCap::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void MarioCap::Load(FILE* File)
{
	DynamicObject::Load(File);
}

void MarioCap::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	AddComponent<CapStateScript>(eComponentType::Script);
	//AddComponent<Transform>(eComponentType::Transform);
	//AddComponent<Transform>(eComponentType::Transform);

	//model initialize
	SetPos(Vector3(0.f, 20.f, 0.f));
	
	SetScale(Vector3(0.01f, 0.01f, 0.01f));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioCap");
	GetComponent<MeshRenderer>()->SetModel(model);
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"marioCapMaterial");

	BoneAnimator* animator = GetComponent<BoneAnimator>(); 
	boneAnimatorInit(animator);

	AddComponent<GenericAnimator>(eComponentType::GenericAnimator);

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.25f, 0.125f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.25f, 0.125f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	physical->KinematicActorSleep();

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

	GetComponent<MeshRenderer>()->SetBoneAnimator(nullptr);
	Pause();
}

void MarioCap::Update()
{
	// owner�� �÷��̾ �ƴ϶�� ������ġ�� ���� ���� �ٲ��ش�.

	if (eLayerType::Player != mOwner->GetLayerType())
	{
		//Matrix capWorldMatrix = {};
		//Matrix ownerWorldMatrix = mOwner->GetTransform()->GetWorldMatrix();

		//
		//Bone* bone = mOwner->GetMeshRenderer()->GetModel()->FindBone(L"Armature_Head");	
		//ModelNode* node = mOwner->GetMeshRenderer()->GetModel()->FindNode(L"Armature_Cap");
		//
		//Matrix boneMat = convert::aiMat44ToMat44(bone->mFinalMatrix);
		//Matrix nodeMat = convert::aiMat44ToMat44(node->mTransformation);
		//mOwner->GetTransform()->SetParent(nodeMat);
		//Vector3 offset = {0.f, 1.25f, 0.f};
		//capWorldMatrix = ownerWorldMatrix;
		//capWorldMatrix._41 = (ownerWorldMatrix._41 + offset.x);
		//capWorldMatrix._42 = (ownerWorldMatrix._42 + offset.y);
		//capWorldMatrix._43 = (ownerWorldMatrix._43 + offset.z);
		//GetTransform()->SetWorldMatrix(nodeMat);

		Transform* tr = GetTransform();
		Transform* OwnerTr = mOwner->GetTransform();
		Vector3 pxPos = OwnerTr->GetPhysicalPosition();
		pxPos.y += 1.5f;
		tr->SetPhysicalPosition(pxPos);
		tr->SetPhysicalRotation(OwnerTr->GetPhysicalRotation());
	}

	DynamicObject::Update();
}

void MarioCap::FixedUpdate()
{
	if (mOwner == nullptr)
		return;

	if (GetCapState() == eCapState::Idle)
	{
		Transform* tr = GetTransform();
		Transform* OwnerTr = mOwner->GetTransform();

		tr->SetPhysicalPosition(OwnerTr->GetPhysicalPosition());
		tr->SetPhysicalRotation(OwnerTr->GetPhysicalRotation());
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

		dynamic_cast<Player*>(GetOwner())->SetPlayerState(Player::ePlayerState::Capture);
		GetComponent<GenericAnimator>()->Stop();
		//Pause();
		//GetOwner()->GetPhysical()->RemoveActorToPxScene();

		// ī�޶��� ������ ĸó ������� �ٲ��ش�.
		renderer::mainCamera->SetTarget(gameObject);
	}

	if (gameObject->GetLayerType() == eLayerType::Objects)
	{
		FlyEnd();
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
	Vector3 pos = mOwner->GetTransform()->GetPhysicalPosition();
	Vector3 playerforward = mOwner->GetTransform()->WorldForward();

	AnimatorParam param;
	// ����Ÿ��
	param.AnimType = eAnimType::Linear;

	// ������ ���� Value ��
	param.StartValue = 0.f;
	param.EndValue= 15.f;

	// ����ð�
	param.DurationTime = 0.5f;

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
		GetPhysical()->KinematicActorSleep();
		Pause();
	};

	// �̺�Ʈ ����
	animator->Start(param);
}
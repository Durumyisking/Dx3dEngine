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

MarioCap::MarioCap()
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
	SetScale(Vector3(0.01f, 0.01f, 0.01f));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioCap");
	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	BoneAnimator* animator = GetComponent<BoneAnimator>(); 
	boneAnimatorInit(animator);

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Sphere, Vector3(0.5f, 1.f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();
	rigidbody->RemoveGravity();
	rigidbody->SetAirOff();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();


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
	animator->CreateAnimation(L"Capture", L"..//..//Resources/MarioCap/Animation/Capture.smd", 0.05f);
	animator->CreateAnimation(L"FlyingStart", L"..//..//Resources/MarioCap/Animation/FlyingStart.smd", 0.05f);
	animator->CreateAnimation(L"FlyingWait", L"..//..//Resources/MarioCap/Animation/FlyingWait.smd", 0.05f);
	//animator->Play(L"Capture");

	//���� ������ flyStart
	{
		cilp = animator->GetAnimationClip(L"ThrowCap");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"FlyingStart");
		});
	}

	//flyStart�� fly
	{
		cilp = animator->GetAnimationClip(L"FlyingStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"FlyingWait");
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


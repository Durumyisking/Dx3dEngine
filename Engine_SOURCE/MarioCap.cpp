#include "MarioCap.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "BoneAnimator.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"
#include "PhysXCollider.h"
#include "PlayerScript.h"
#include "Transform.h"

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
	//AddComponent<Transform>(eComponentType::Transform);

	//model initialize
	SetPos(Vector3(0.f, 0.f, 0.f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioCap");
	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

	BoneAnimator* animator = GetComponent<BoneAnimator>(); 
	boneAnimatorInit(animator);


	//model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
	//model->SetParentTargetBone(L"Armature_Cap");
	//model->SetTargetBone(L"Armature_nw4f_root");
	//model->SetOffsetRotation(Vector3(0.0f, 1.570796f, -1.570796f * 2));


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
	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();
	rigidbody->RemoveGravity();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

}

void MarioCap::OnCollisionEnter(GameObj* gameObject)
{
}

void MarioCap::OnTriggerEnter(GameObj* gameObject)
{
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
	animator->Play(L"Capture");

	//모자 던진후 flyStart
	{
		cilp = animator->GetAnimationClip(L"ThrowCap");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"FlyingStart");
		});
	}

	//flyStart후 fly
	{
		cilp = animator->GetAnimationClip(L"FlyingStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"FlyingWait");
		});
	}
}


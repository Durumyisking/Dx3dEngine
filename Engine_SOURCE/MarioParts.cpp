#include "MarioParts.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "BoneAnimator.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"
#include "PhysXCollider.h"
#include "PlayerScript.h"
#include "Transform.h"

MarioParts::MarioParts()
{
}

MarioParts::~MarioParts()
{
}

void MarioParts::Initialize()
{
}

void MarioParts::Update()
{
	//DynamicObject::Update();
	//mMarioPhysicPos = mBody->GetComponent<Transform>()->GetPhysicalPosition();
	//for (auto& i : mMario)
	//{
	//	i->GetComponent<Transform>()->SetPhysicalPosition(mMarioPhysicPos);
	//}

}

void MarioParts::FixedUpdate()
{
}

void MarioParts::Render()
{
}

void MarioParts::FontRender()
{
}

void MarioParts::OnCollisionEnter(GameObj* gameObject)
{
}

void MarioParts::OnTriggerEnter(GameObj* gameObject)
{
}

void MarioParts::OnTriggerExit(GameObj* gameObject)
{
}


void MarioParts::BoneInitialize()
{
	{
		//mBody->SetPos(Vector3(0.f, 0.f, 0.f));
		//mBody->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		//mBody->SetName(L"mBody");
		
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
		mBody->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = mBody->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

		//animator->LoadAnimations(L"..//Resources/MarioBody/Animation");
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioBody/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioBody/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioBody/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioBody/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		mBody->GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");
		mBody->AddComponent<PlayerScript>(eComponentType::Script);
		mBody->AddComponent<Transform>(eComponentType::Transform);

		Physical* physical = mBody->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.5f, 0.5f, 0.5f));

		PhysXRigidBody* rigid = mBody->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		mBody->AddComponent<PhysXCollider>(eComponentType::Collider);
		mBody->AddComponent<PhysicalMovement>(eComponentType::Movement);

		mMarioPhysicPos = mBody->GetComponent<Transform>()->GetPhysicalPosition();
	}


	{
		MarioParts* parts = new MarioParts;
		parts->SetPos(Vector3(0.f, 0.f, 0.f));
		parts->SetScale(Vector3(1.0f, 1.f, 1.0f));
		parts->SetName(L"parts");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandL");
		parts->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = parts->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandL/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandL/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandL/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandL/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandL");
		model->SetTargetBone(L"Armature_HandL");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
		mMario.push_back(parts);

	}

	{
		MarioParts* parts = new MarioParts;
		parts->SetPos(Vector3(0.f, 0.f, 0.f));
		parts->SetScale(Vector3(1.0f, 1.f, 1.0f));
		parts->SetName(L"parts");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandR");
		parts->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = parts->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandR/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandR/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandR/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandR/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandR");
		model->SetTargetBone(L"Armature_HandR");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
		mMario.push_back(parts);

	}

	{
		MarioParts* parts = new MarioParts;
		parts->SetPos(Vector3(0.f, 0.f, 0.f));
		parts->SetScale(Vector3(1.0f, 1.f, 1.0f));
		parts->SetName(L"parts");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
		parts->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = parts->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHead/Animation/Defalut.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_MarioHead");

		model->SetOffsetRotation(Vector3(0.0f, 1.570796f, -1.570796f * 2));
		mMario.push_back(parts);

	}

	{
		MarioParts* parts = new MarioParts;
		parts->SetPos(Vector3(0.f, 0.f, 0.f));
		parts->SetScale(Vector3(1.0f, 1.f, 1.0f));
		parts->SetName(L"parts");

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioFace");
		parts->GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = parts->AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioFace/Animation/AreaWaitStink.smd", 0.02f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioFace/Animation/Walk.smd", 0.02f);
		animator->Play(L"test2");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_Face");

		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, -1.570796f * 2));
		mMario.push_back(parts);

	}
}


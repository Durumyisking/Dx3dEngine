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
	SetLayerType(eLayerType::Player);

}

MarioParts::~MarioParts()
{
}

void MarioParts::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	//Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	//physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(0.000001f, 0.000001f, 0.000001f));
	//AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	AddComponent<Transform>(eComponentType::Transform);
	//AddComponent<PhysXCollider>(eComponentType::Collider);
	//AddComponent<PhysicalMovement>(eComponentType::Movement);

	BoneInitialize();
	DynamicObject::Initialize();
}

void MarioParts::Update()
{
	DynamicObject::Update();
}

void MarioParts::FixedUpdate()
{
	DynamicObject::FixedUpdate();
}

void MarioParts::Render()
{
	DynamicObject::Render();
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
	if (GetName() == L"HandL") {

		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(1.0f, 1.f, 1.0f));

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandL");
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = GetComponent<BoneAnimator>();
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandL/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandL/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandL/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandL/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandL");
		model->SetTargetBone(L"Armature_HandL");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
	}

	if (GetName() == L"HandR") {

		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(1.0f, 1.f, 1.0f));

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHandR");
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = GetComponent<BoneAnimator>();
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHandR/Animation/Walk.smd", 0.05f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioHandR/Animation/Jump.smd", 0.05f);
		animator->CreateAnimation(L"test3", L"..//..//Resources/MarioHandR/Animation/Dead.smd", 0.05f);
		animator->CreateAnimation(L"test4", L"..//..//Resources/MarioHandR/Animation/Run.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_HandR");
		model->SetTargetBone(L"Armature_HandR");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
	}

	if (GetName() == L"Head") {
		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(1.0f, 1.f, 1.0f));

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = GetComponent<BoneAnimator>();
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioHead/Animation/Defalut.smd", 0.05f);
		animator->Play(L"test");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_MarioHead");
		model->SetOffsetRotation(Vector3(0.0f, 1.570796f, -1.570796f * 2));


	}

	if (GetName() == L"Face") {
		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(1.0f, 1.f, 1.0f));

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioFace");
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = GetComponent<BoneAnimator>();
		animator->CreateAnimation(L"test", L"..//..//Resources/MarioFace/Animation/AreaWaitStink.smd", 0.02f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioFace/Animation/Walk.smd", 0.02f);
		animator->Play(L"test2");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"Mario"));
		model->SetParentTargetBone(L"Armature_Head");
		model->SetTargetBone(L"Armature_Face");
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, -1.570796f * 2));

	}

	if (GetName() == L"Eye") {
		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(1.0f, 1.f, 1.0f));

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioEye");
		GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));

		BoneAnimator* animator = GetComponent<BoneAnimator>();
		//animator->CreateAnimation(L"test", L"..//..//Resources/MarioFace/Animation/AreaWaitStink.smd", 0.02f);
		animator->CreateAnimation(L"test2", L"..//..//Resources/MarioEye/Animation/Wait.smd", 0.02f);
		animator->Play(L"test2");

		model->SetParentModel(GETSINGLE(ResourceMgr)->Find<Model>(L"MarioFace"));
		model->SetParentTargetBone(L"Armature_Face");
		model->SetTargetBone(L"Armature_nw4f_root");
		//model->SetOffsetRotation(Vector3(0.0f, -1.570796f, -1.570796f * 2));
		model->SetOffsetRotation(Vector3(0.0f, -1.570796f, 0.0f));
		model->MeshRenderSwtich(L"EyelidCloseLeft__EyeMT", false);
		model->MeshRenderSwtich(L"EyelidCloseRight__EyeMT", false);
		//model->MeshRenderSwtich(L"EyelidLeft__EyeMT", false);
		//model->MeshRenderSwtich(L"EyelidRight__EyeMT", false);
	}
}


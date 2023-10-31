#include "EffectObject.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"
#include "Model.h"
#include "Physical.h"
#include "PhysicalMovement.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "TimeMgr.h"

EffectObject::EffectObject()
{
}

EffectObject::~EffectObject()
{
}

void EffectObject::Initialize()
{
	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	if (!meshRenderer)
		return;

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"PackunBall");
	if (model)
	{
		// Model Setting
		meshRenderer->SetModel(model, model->GetMaterial(0));
	}

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));
	physical->CreateSubShape(Vector3::Zero, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);


	// Rigidbody
	assert(AddComponent<PhysXRigidBody>(eComponentType::RigidBody));

	// MoveMent
	assert(AddComponent<PhysXCollider>(eComponentType::Collider));
	assert(AddComponent<PhysicalMovement>(eComponentType::Movement));

	GameObj::Initialize();
	//test
	//GetComponent<Transform>()->SetPhysicalPosition(Vector3(-500.f, 0.0f, 1.0f));
}

void EffectObject::Update()
{
	GameObj::Update();
}

void EffectObject::OnTriggerEnter(GameObj* gameObject)
{
	if (!gameObject)
		return;

	if (gameObject->GetLayerType() == eLayerType::Platforms && GetPhysXRigidBody()->GetVelocity().y < 0.f)
	{
		Pause();
		//GetPhysical()->ShapesPause();
	}
}

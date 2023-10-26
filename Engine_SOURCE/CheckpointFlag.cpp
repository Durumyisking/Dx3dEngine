#include "CheckpointFlag.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


CheckpointFlag::CheckpointFlag()
{

}

CheckpointFlag::~CheckpointFlag()
{

}

void CheckpointFlag::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CheckpointFlag");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CheckpointFlag_0Material", 0);
	mr->SetMaterialByKey(L"CheckpointFlag_1Material", 1);
	mr->SetMaterialByKey(L"CheckpointFlag_3Material", 2);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void CheckpointFlag::Update()
{
	GameObj::Update();
}

void CheckpointFlag::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CheckpointFlag::OnCollisionEnter(GameObj* gameObject)
{
}

void CheckpointFlag::OnTriggerEnter(GameObj* gameObject)
{
}

void CheckpointFlag::OnTriggerPersist(GameObj* gameObject)
{
}

void CheckpointFlag::OnTriggerExit(GameObj* gameObject)
{
}

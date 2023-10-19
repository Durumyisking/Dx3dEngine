#include "BenchA.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


BenchA::BenchA()
{

}

BenchA::~BenchA()
{

}

void BenchA::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBench000");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"BenchiWood00Material");

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void BenchA::Update()
{
	GameObj::Update();
}

void BenchA::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void BenchA::OnCollisionEnter(GameObj* gameObject)
{
}

void BenchA::OnTriggerEnter(GameObj* gameObject)
{
}

void BenchA::OnTriggerPersist(GameObj* gameObject)
{
}

void BenchA::OnTriggerExit(GameObj* gameObject)
{
}

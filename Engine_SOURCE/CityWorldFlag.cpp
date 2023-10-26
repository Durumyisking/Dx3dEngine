#include "CityWorldFlag.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CityWorldFlag::CityWorldFlag()
{

}

CityWorldFlag::~CityWorldFlag()
{

}

void CityWorldFlag::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldFlag");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CItyWorldFlag_1Material", 0);
	mr->SetMaterialByKey(L"CItyWorldFlag_0Material", 1);
	mr->SetMaterialByKey(L"CItyWorldFlag_1Material", 2);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void CityWorldFlag::Update()
{
	GameObj::Update();
}

void CityWorldFlag::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityWorldFlag::OnCollisionEnter(GameObj* gameObject)
{
}

void CityWorldFlag::OnTriggerEnter(GameObj* gameObject)
{
}

void CityWorldFlag::OnTriggerPersist(GameObj* gameObject)
{
}

void CityWorldFlag::OnTriggerExit(GameObj* gameObject)
{
}

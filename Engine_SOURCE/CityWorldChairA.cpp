#include "CityWorldChairA.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CityWorldChairA::CityWorldChairA()
{
}

CityWorldChairA::~CityWorldChairA()
{
}

void CityWorldChairA::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldChairA");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CityWorldChairA_0Material");

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void CityWorldChairA::Update()
{
	GameObj::Update();
}

void CityWorldChairA::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityWorldChairA::OnCollisionEnter(GameObj* gameObject)
{
}

void CityWorldChairA::OnTriggerEnter(GameObj* gameObject)
{
}

void CityWorldChairA::OnTriggerPersist(GameObj* gameObject)
{
}

void CityWorldChairA::OnTriggerExit(GameObj* gameObject)
{
}

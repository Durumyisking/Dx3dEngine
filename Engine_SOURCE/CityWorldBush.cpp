#include "CityWorldBush.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CityWorldBush::CityWorldBush()
{
}

CityWorldBush::~CityWorldBush()
{
}

void CityWorldBush::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldBush");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CityWorldBush_0Material", 0);
	mr->SetMaterialByKey(L"CityWorldBush_1Material", 1);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);



	GameObj::Initialize();
}

void CityWorldBush::Update()
{
	GameObj::Update();
}

void CityWorldBush::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityWorldBush::OnCollisionEnter(GameObj* gameObject)
{
}

void CityWorldBush::OnTriggerEnter(GameObj* gameObject)
{
}

void CityWorldBush::OnTriggerPersist(GameObj* gameObject)
{
}

void CityWorldBush::OnTriggerExit(GameObj* gameObject)
{
}

#include "Building.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

Building::Building()
{
}

Building::~Building()
{

}

void Building::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding002");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"GlassBuildingWallMaterial",0);
	mr->SetMaterialByKey(L"RoofConcrete00Material",1);
	mr->SetMaterialByKey(L"RoofConcrete01Material",2);
	mr->SetMaterialByKey(L"WallGlassPaintedSteelMaterial", 3);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, {10.f, 50.f, 10.f});
	
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void Building::Update()
{
	GameObj::Update();
}

void Building::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Building::OnCollisionEnter(GameObj* gameObject)
{
}

void Building::OnTriggerEnter(GameObj* gameObject)
{
}

void Building::OnTriggerPersist(GameObj* gameObject)
{
}

void Building::OnTriggerExit(GameObj* gameObject)
{
}

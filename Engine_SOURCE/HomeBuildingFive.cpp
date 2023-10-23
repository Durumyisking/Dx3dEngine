#include "HomeBuildingFive.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuildingFive::HomeBuildingFive()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuildingFive";
}

HomeBuildingFive::HomeBuildingFive(const HomeBuildingFive& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuildingFive::~HomeBuildingFive()
{
}

HomeBuildingFive* HomeBuildingFive::Clone() const
{
	return new HomeBuildingFive(*this);
}

void HomeBuildingFive::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding005");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding005_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding005_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding005_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding005_1Material", 3); //수정 필요 BuildingEntrance02
	mr->SetMaterialByKey(L"HomeBuilding005_3Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding005_4Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding005_5Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding005_6Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding005_7Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding005_8Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding005_9Material", 10);
	mr->SetMaterialByKey(L"HomeBuilding005_10Material", 11);
	mr->SetMaterialByKey(L"HomeBuilding005_11Material", 12);
	mr->SetMaterialByKey(L"HomeBuilding005_13Material", 13);
	mr->SetMaterialByKey(L"HomeBuilding005_12Material", 14);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuildingFive::Update()
{
	GameObj::Update();
}

void HomeBuildingFive::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingFive::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerExit(GameObj* gameObject)
{
}

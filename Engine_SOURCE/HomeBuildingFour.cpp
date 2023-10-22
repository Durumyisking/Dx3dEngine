#include "HomeBuildingFour.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuildingFour::HomeBuildingFour()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuildingFour";
}

HomeBuildingFour::HomeBuildingFour(const HomeBuildingFour& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuildingFour::~HomeBuildingFour()
{
}

HomeBuildingFour* HomeBuildingFour::Clone() const
{
	return new HomeBuildingFour(*this);
}

void HomeBuildingFour::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding004");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding004_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding004_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding004_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding004_1Material", 3); //수정 필요 BuildingEntrance02
	mr->SetMaterialByKey(L"HomeBuilding004_3Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding004_4Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding004_5Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding004_6Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding004_7Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding004_8Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding004_9Material", 10);
	mr->SetMaterialByKey(L"HomeBuilding004_10Material", 11);
	mr->SetMaterialByKey(L"HomeBuilding004_11Material", 12);
	mr->SetMaterialByKey(L"HomeBuilding004_13Material", 13);
	mr->SetMaterialByKey(L"HomeBuilding004_12Material", 14);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuildingFour::Update()
{
	GameObj::Update();
}

void HomeBuildingFour::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingFour::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerExit(GameObj* gameObject)
{
}

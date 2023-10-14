#include "CityGround.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


CityGround::CityGround()
{
}

CityGround::~CityGround()
{
}

void CityGround::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityGround");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"BlueseatStepRepeat00Material", 0);
	mr->SetMaterialByKey(L"MetalScaffoldRepeat00Material", 1);
	mr->SetMaterialByKey(L"RoadManhole00Material", 2);
	mr->SetMaterialByKey(L"MetalScaffold00Material", 3);
	mr->SetMaterialByKey(L"GrassGround00Material", 4);
	mr->SetMaterialByKey(L"GroundEarth01Material", 5);
	mr->SetMaterialByKey(L"SideWalk00Material", 6);
	mr->SetMaterialByKey(L"GroundCityLogo00Material", 7);
	mr->SetMaterialByKey(L"GroundCityLogo00Material", 8);
	mr->SetMaterialByKey(L"GroundMoon01Material", 9);
	mr->SetMaterialByKey(L"GroundCityLogo02Material", 10);
	mr->SetMaterialByKey(L"AsphaltSeal00Material", 11);
	mr->SetMaterialByKey(L"GroundLight00Material", 12);
	mr->SetMaterialByKey(L"GroundTile00Material", 13);
	mr->SetMaterialByKey(L"GroundTile01Material", 14);
	mr->SetMaterialByKey(L"MetalManhole03Material", 15);
	mr->SetMaterialByKey(L"GroundSoil00Material", 16);
	mr->SetMaterialByKey(L"GroundLawn00Material", 17);
	mr->SetMaterialByKey(L"GroundLawn01Material", 18);
	mr->SetMaterialByKey(L"GroundLawn01Material", 19);
	mr->SetMaterialByKey(L"SideWalk01Material", 20);
	mr->SetMaterialByKey(L"SideWalk02Material", 21);
	mr->SetMaterialByKey(L"ConcreteWall00Material", 22);
	mr->SetMaterialByKey(L"MetalManhole01Material", 23);
	mr->SetMaterialByKey(L"Font_BaseRoadSign00Material", 24);
	mr->SetMaterialByKey(L"CityLitter01Material", 25);
	mr->SetMaterialByKey(L"CityLitter00Material", 26);
	mr->SetMaterialByKey(L"Font_BaseRoadSign01Material", 27);
	mr->SetMaterialByKey(L"DrainageCover00Material", 28);
	mr->SetMaterialByKey(L"WallGrunge00Material", 29);
	mr->SetMaterialByKey(L"WallFastener00Material", 30);
	mr->SetMaterialByKey(L"WallGrunge01Material", 31);
	mr->SetMaterialByKey(L"RoadAsphaltGlossy00Material", 32);
	mr->SetMaterialByKey(L"BaseAsphaltRoad01Material", 33);
	mr->SetMaterialByKey(L"GrassFlowerSet00Material", 34);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);



	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);



	GameObj::Initialize();
}

void CityGround::Update()
{
	GameObj::Update();
}

void CityGround::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityGround::OnCollisionEnter(GameObj* gameObject)
{
}

void CityGround::OnTriggerEnter(GameObj* gameObject)
{
}

void CityGround::OnTriggerPersist(GameObj* gameObject)
{
}

void CityGround::OnTriggerExit(GameObj* gameObject)
{
}

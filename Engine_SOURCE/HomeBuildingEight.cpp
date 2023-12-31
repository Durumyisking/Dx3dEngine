#include "HomeBuildingEight.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuildingEight::HomeBuildingEight()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuildingEight";
}

HomeBuildingEight::HomeBuildingEight(const HomeBuildingEight& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuildingEight::~HomeBuildingEight()
{

}

HomeBuildingEight* HomeBuildingEight::Clone() const
{
	return new HomeBuildingEight(*this);
}

void HomeBuildingEight::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding008");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);


	mr->SetMaterialByKey(L"HomeBuilding008_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding008_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding008_2Material", 2);  //���� �ʿ� GlassBuilding1F01
	mr->SetMaterialByKey(L"HomeBuilding008_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding008_2Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding008_3Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding008_4Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding008_5Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding008_6Material", 8);


	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuildingEight::Update()
{
	GameObj::Update();
}

void HomeBuildingEight::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingEight::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerExit(GameObj* gameObject)
{
}
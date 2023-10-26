#include "HomeBuilding_12.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_12::HomeBuilding_12()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_12";
}

HomeBuilding_12::HomeBuilding_12(const HomeBuilding_12& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_12::~HomeBuilding_12()
{
}

HomeBuilding_12* HomeBuilding_12::Clone() const
{
	return new HomeBuilding_12(*this);
}

void HomeBuilding_12::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding012");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0012_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0012_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0012_3Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0012_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0012_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0012_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0012_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0012_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0012_8Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding0012_9Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding0012_9Material", 10);
	mr->SetMaterialByKey(L"HomeBuilding0012_10Material", 11);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_12::Update()
{
	GameObj::Update();
}

void HomeBuilding_12::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_12::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerExit(GameObj* gameObject)
{
}

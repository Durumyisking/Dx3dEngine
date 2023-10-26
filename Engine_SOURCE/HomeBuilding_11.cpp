#include "HomeBuilding_11.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_11::HomeBuilding_11()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_11";
}

HomeBuilding_11::HomeBuilding_11(const HomeBuilding_11& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_11::~HomeBuilding_11()
{
}

HomeBuilding_11* HomeBuilding_11::Clone() const
{
	return new HomeBuilding_11(*this);
}

void HomeBuilding_11::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding011");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0011_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0011_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0011_3Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0011_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0011_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0011_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0011_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0011_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0011_8Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding0011_9Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding0011_9Material", 10);
	mr->SetMaterialByKey(L"HomeBuilding0011_10Material", 11);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_11::Update()
{
	GameObj::Update();
}

void HomeBuilding_11::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_11::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerExit(GameObj* gameObject)
{
}

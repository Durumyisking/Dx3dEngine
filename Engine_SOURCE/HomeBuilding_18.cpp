#include "HomeBuilding_18.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_18::HomeBuilding_18()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_18";
}

HomeBuilding_18::HomeBuilding_18(const HomeBuilding_18& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_18::~HomeBuilding_18()
{
}

HomeBuilding_18* HomeBuilding_18::Clone() const
{
	return new HomeBuilding_18(*this);
}

void HomeBuilding_18::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding018");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0018_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0018_0Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0018_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0018_10Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0018_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0018_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0018_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0018_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0018_8Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding0018_9Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding0018_10Material", 10);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_18::Update()
{
}

void HomeBuilding_18::FixedUpdate()
{
}

void HomeBuilding_18::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_18::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_18::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_18::OnTriggerExit(GameObj* gameObject)
{
}

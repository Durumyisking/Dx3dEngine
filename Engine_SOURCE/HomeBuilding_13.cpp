#include "HomeBuilding_13.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_13::HomeBuilding_13()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuilding_13";
}

HomeBuilding_13::HomeBuilding_13(const HomeBuilding_13& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuilding_13::~HomeBuilding_13()
{
}

HomeBuilding_13* HomeBuilding_13::Clone() const
{
	return new HomeBuilding_13(*this);
}

void HomeBuilding_13::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding013");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0013_1Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0013_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0013_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0013_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0013_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0013_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0013_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0013_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0013_7Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding0013_8Material", 9);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_13::Update()
{
	GameObj::Update();
}

void HomeBuilding_13::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_13::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerExit(GameObj* gameObject)
{
}

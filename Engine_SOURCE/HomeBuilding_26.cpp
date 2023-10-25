#include "HomeBuilding_26.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_26::HomeBuilding_26()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuilding_26";
}

HomeBuilding_26::HomeBuilding_26(const HomeBuilding_26& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuilding_26::~HomeBuilding_26()
{
}

HomeBuilding_26* HomeBuilding_26::Clone() const
{
	return new HomeBuilding_26(*this);
}

void HomeBuilding_26::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding026");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0026_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0026_2Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0026_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0026_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0026_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0026_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0026_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0026_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0026_8Material", 8);
	mr->SetMaterialByKey(L"HomeBuilding0026_9Material", 9);
	mr->SetMaterialByKey(L"HomeBuilding0026_10Material", 10);
	mr->SetMaterialByKey(L"HomeBuilding0026_11Material", 11);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 3.f,1.f,3.f });
	//physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 3.f,1.f,3.f }, { 0.f, 1000.f, 0.f }, MassProperties()); ::  Àû¿ë¾ÈµÊ

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_26::Update()
{
	GameObj::Update();
}

void HomeBuilding_26::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_26::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_26::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_26::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_26::OnTriggerExit(GameObj* gameObject)
{
}

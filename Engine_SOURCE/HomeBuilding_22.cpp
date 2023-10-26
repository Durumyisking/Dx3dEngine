#include "HomeBuilding_22.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuilding_22::HomeBuilding_22()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_22";
}

HomeBuilding_22::HomeBuilding_22(const HomeBuilding_22& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_22::~HomeBuilding_22()
{
}

HomeBuilding_22* HomeBuilding_22::Clone() const
{
	return new HomeBuilding_22(*this);
}

void HomeBuilding_22::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding022");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0022_7Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0022_9Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0022_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0022_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0022_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0022_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0022_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0022_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0022_8Material", 8);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_22::Update()
{
	GameObj::Update();
}

void HomeBuilding_22::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_22::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_22::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_22::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_22::OnTriggerExit(GameObj* gameObject)
{
}

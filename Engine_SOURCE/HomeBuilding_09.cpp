#include "HomeBuilding_09.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_09::HomeBuilding_09()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuilding_09";
}


HomeBuilding_09::HomeBuilding_09(const HomeBuilding_09& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuilding_09::~HomeBuilding_09()
{
}

HomeBuilding_09* HomeBuilding_09::Clone() const
{
	return new HomeBuilding_09(*this);
}

void HomeBuilding_09::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding009");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding009_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding009_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding009_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding009_1Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding009_2Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding009_3Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding009_4Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding009_5Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding009_6Material", 8);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_09::Update()
{
	GameObj::Update();
}

void HomeBuilding_09::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_09::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_09::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_09::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_09::OnTriggerExit(GameObj* gameObject)
{
}

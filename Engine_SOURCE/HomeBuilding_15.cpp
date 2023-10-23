#include "HomeBuilding_15.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_15::HomeBuilding_15()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuilding_15";
}

HomeBuilding_15::HomeBuilding_15(const HomeBuilding_15& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuilding_15::~HomeBuilding_15()
{
}

HomeBuilding_15* HomeBuilding_15::Clone() const
{
	return new HomeBuilding_15(*this);
}

void HomeBuilding_15::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding015");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding002_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding002_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding002_0Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding002_3Material", 3);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_15::Update()
{
	GameObj::Update();
}

void HomeBuilding_15::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_15::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_15::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_15::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_15::OnTriggerExit(GameObj* gameObject)
{
}

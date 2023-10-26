#include "HomeBuilding_16.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_16::HomeBuilding_16()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_16";
}

HomeBuilding_16::HomeBuilding_16(const HomeBuilding_16& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_16::~HomeBuilding_16()
{
}

HomeBuilding_16* HomeBuilding_16::Clone() const
{
	return new HomeBuilding_16(*this);
}

void HomeBuilding_16::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding016");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0016_7Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0016_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0016_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0016_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0016_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0016_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0016_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0016_7Material", 7);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_16::Update()
{
	GameObj::Update();
}

void HomeBuilding_16::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_16::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_16::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_16::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_16::OnTriggerExit(GameObj* gameObject)
{
}

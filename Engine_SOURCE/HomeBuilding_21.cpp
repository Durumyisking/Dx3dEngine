#include "HomeBuilding_21.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_21::HomeBuilding_21()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuilding_21";
}

HomeBuilding_21::HomeBuilding_21(const HomeBuilding_21& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuilding_21::~HomeBuilding_21()
{
}

HomeBuilding_21* HomeBuilding_21::Clone() const
{
	return new HomeBuilding_21(*this);
}

void HomeBuilding_21::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding021");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding0021_7Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding0021_9Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding0021_2Material", 2);
	mr->SetMaterialByKey(L"HomeBuilding0021_3Material", 3);
	mr->SetMaterialByKey(L"HomeBuilding0021_4Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding0021_5Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding0021_6Material", 6);
	mr->SetMaterialByKey(L"HomeBuilding0021_7Material", 7);
	mr->SetMaterialByKey(L"HomeBuilding0021_8Material", 8);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_21::Update()
{
	GameObj::Update();
}

void HomeBuilding_21::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_21::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerExit(GameObj* gameObject)
{
}

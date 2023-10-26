#include "HomeBuildingFive.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuildingFive::HomeBuildingFive()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuildingFive";
}

HomeBuildingFive::HomeBuildingFive(const HomeBuildingFive& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuildingFive::~HomeBuildingFive()
{
}

HomeBuildingFive* HomeBuildingFive::Clone() const
{
	return new HomeBuildingFive(*this);
}

void HomeBuildingFive::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding005");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->ForceSetMaterial(model->GetLastMaterial());

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuildingFive::Update()
{
	GameObj::Update();
}

void HomeBuildingFive::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingFive::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingFive::OnTriggerExit(GameObj* gameObject)
{
}

#include "HomeBuildingFour.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuildingFour::HomeBuildingFour()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuildingFour";
}

HomeBuildingFour::HomeBuildingFour(const HomeBuildingFour& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuildingFour::~HomeBuildingFour()
{
}

HomeBuildingFour* HomeBuildingFour::Clone() const
{
	return new HomeBuildingFour(*this);
}

void HomeBuildingFour::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding004");
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

void HomeBuildingFour::Update()
{
	GameObj::Update();
}

void HomeBuildingFour::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingFour::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingFour::OnTriggerExit(GameObj* gameObject)
{
}

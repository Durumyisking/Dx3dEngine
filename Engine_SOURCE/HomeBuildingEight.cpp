#include "HomeBuildingEight.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuildingEight::HomeBuildingEight()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuildingEight";
}

HomeBuildingEight::HomeBuildingEight(const HomeBuildingEight& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuildingEight::~HomeBuildingEight()
{

}

HomeBuildingEight* HomeBuildingEight::Clone() const
{
	return new HomeBuildingEight(*this);
}

void HomeBuildingEight::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding008");
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

void HomeBuildingEight::Update()
{
	GameObj::Update();
}

void HomeBuildingEight::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingEight::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingEight::OnTriggerExit(GameObj* gameObject)
{
}
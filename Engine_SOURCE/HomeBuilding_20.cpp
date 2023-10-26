#include "HomeBuilding_20.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_20::HomeBuilding_20()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_20";
}

HomeBuilding_20::HomeBuilding_20(const HomeBuilding_20& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_20::~HomeBuilding_20()
{
}

HomeBuilding_20* HomeBuilding_20::Clone() const
{
	return new HomeBuilding_20(*this);
}

void HomeBuilding_20::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding020");
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

void HomeBuilding_20::Update()
{
}

void HomeBuilding_20::FixedUpdate()
{
}

void HomeBuilding_20::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_20::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_20::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_20::OnTriggerExit(GameObj* gameObject)
{
}

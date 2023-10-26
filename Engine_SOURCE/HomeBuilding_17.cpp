#include "HomeBuilding_17.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_17::HomeBuilding_17()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_17";
}

HomeBuilding_17::HomeBuilding_17(const HomeBuilding_17& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_17::~HomeBuilding_17()
{
}

HomeBuilding_17* HomeBuilding_17::Clone() const
{
	return new HomeBuilding_17(*this);
}

void HomeBuilding_17::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding017");
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

void HomeBuilding_17::Update()
{
}

void HomeBuilding_17::FixedUpdate()
{
}

void HomeBuilding_17::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_17::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_17::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_17::OnTriggerExit(GameObj* gameObject)
{
}

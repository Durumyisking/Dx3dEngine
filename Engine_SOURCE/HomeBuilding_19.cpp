#include "HomeBuilding_19.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_19::HomeBuilding_19()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_19";
}

HomeBuilding_19::HomeBuilding_19(const HomeBuilding_19& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_19::~HomeBuilding_19()
{
}

HomeBuilding_19* HomeBuilding_19::Clone() const
{
	return new HomeBuilding_19(*this);
}

void HomeBuilding_19::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding019");
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

void HomeBuilding_19::Update()
{
}

void HomeBuilding_19::FixedUpdate()
{
}

void HomeBuilding_19::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_19::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_19::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_19::OnTriggerExit(GameObj* gameObject)
{
}

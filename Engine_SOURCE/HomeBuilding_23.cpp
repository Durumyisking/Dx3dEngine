#include "HomeBuilding_23.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_23::HomeBuilding_23()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_23";
}

HomeBuilding_23::HomeBuilding_23(const HomeBuilding_23& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_23::~HomeBuilding_23()
{
}

HomeBuilding_23* HomeBuilding_23::Clone() const
{
	return new HomeBuilding_23(*this);
}

void HomeBuilding_23::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding023");
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

void HomeBuilding_23::Update()
{
	GameObj::Update();
}

void HomeBuilding_23::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_23::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_23::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_23::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_23::OnTriggerExit(GameObj* gameObject)
{
}

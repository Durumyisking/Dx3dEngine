#include "HomeBuilding_11.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_11::HomeBuilding_11()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_11";
}

HomeBuilding_11::HomeBuilding_11(const HomeBuilding_11& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_11::~HomeBuilding_11()
{
}

HomeBuilding_11* HomeBuilding_11::Clone() const
{
	return new HomeBuilding_11(*this);
}

void HomeBuilding_11::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding011");
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

void HomeBuilding_11::Update()
{
	GameObj::Update();
}

void HomeBuilding_11::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_11::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_11::OnTriggerExit(GameObj* gameObject)
{
}

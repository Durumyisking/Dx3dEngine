#include "HomeBuilding_12.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_12::HomeBuilding_12()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_12";
}

HomeBuilding_12::HomeBuilding_12(const HomeBuilding_12& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_12::~HomeBuilding_12()
{
}

HomeBuilding_12* HomeBuilding_12::Clone() const
{
	return new HomeBuilding_12(*this);
}

void HomeBuilding_12::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding012");
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

void HomeBuilding_12::Update()
{
	GameObj::Update();
}

void HomeBuilding_12::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_12::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_12::OnTriggerExit(GameObj* gameObject)
{
}

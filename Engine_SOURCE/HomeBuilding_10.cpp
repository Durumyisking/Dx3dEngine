#include "HomeBuilding_10.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_10::HomeBuilding_10()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_10";
}

HomeBuilding_10::HomeBuilding_10(const HomeBuilding_10& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_10::~HomeBuilding_10()
{
}

HomeBuilding_10* HomeBuilding_10::Clone() const
{
	return new HomeBuilding_10(*this);
}

void HomeBuilding_10::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding010");
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

void HomeBuilding_10::Update()
{
	GameObj::Update();
}

void HomeBuilding_10::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_10::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_10::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_10::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_10::OnTriggerExit(GameObj* gameObject)
{
}

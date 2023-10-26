#include "HomeBuilding_24.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


HomeBuilding_24::HomeBuilding_24()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_24";
}

HomeBuilding_24::HomeBuilding_24(const HomeBuilding_24& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_24::~HomeBuilding_24()
{
}

HomeBuilding_24* HomeBuilding_24::Clone() const
{
	return new HomeBuilding_24(*this);
}

void HomeBuilding_24::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding024");
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

void HomeBuilding_24::Update()
{
	GameObj::Update();
}

void HomeBuilding_24::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_24::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_24::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_24::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_24::OnTriggerExit(GameObj* gameObject)
{
}

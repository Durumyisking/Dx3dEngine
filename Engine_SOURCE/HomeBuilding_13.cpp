#include "HomeBuilding_13.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_13::HomeBuilding_13()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_13";
}

HomeBuilding_13::HomeBuilding_13(const HomeBuilding_13& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_13::~HomeBuilding_13()
{
}

HomeBuilding_13* HomeBuilding_13::Clone() const
{
	return new HomeBuilding_13(*this);
}

void HomeBuilding_13::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding013");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->ForceSetMaterial(model->GetLastMaterial());

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 10.f, 25.f, 10.f }, Vector3( 0.f, 25.f, 0.f ));

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuilding_13::Update()
{
	GameObj::Update();
}

void HomeBuilding_13::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_13::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_13::OnTriggerExit(GameObj* gameObject)
{
}

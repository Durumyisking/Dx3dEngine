#include "HomeBuilding_21.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuilding_21::HomeBuilding_21()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeBuilding_21";
}

HomeBuilding_21::HomeBuilding_21(const HomeBuilding_21& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeBuilding_21::~HomeBuilding_21()
{
}

HomeBuilding_21* HomeBuilding_21::Clone() const
{
	return new HomeBuilding_21(*this);
}

void HomeBuilding_21::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding021");
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

void HomeBuilding_21::Update()
{
	GameObj::Update();
}

void HomeBuilding_21::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuilding_21::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuilding_21::OnTriggerExit(GameObj* gameObject)
{
}

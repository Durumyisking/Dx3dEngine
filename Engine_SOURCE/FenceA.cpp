#include "FenceA.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

FenceA::FenceA()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "FenceA";
}

FenceA::FenceA(const FenceA& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

FenceA::~FenceA()
{
}

FenceA* FenceA::Clone() const
{
	return new FenceA(*this);
}

void FenceA::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldFenceA");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"FenceA_0Material", 0);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void FenceA::Update()
{
	GameObj::Update();
}

void FenceA::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void FenceA::OnCollisionEnter(GameObj* gameObject)
{
}

void FenceA::OnTriggerEnter(GameObj* gameObject)
{
}

void FenceA::OnTriggerPersist(GameObj* gameObject)
{
}

void FenceA::OnTriggerExit(GameObj* gameObject)
{
}

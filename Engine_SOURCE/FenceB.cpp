#include "FenceB.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

FenceB::FenceB()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "FenceB";
}

FenceB::FenceB(const FenceB& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

FenceB::~FenceB()
{
}

FenceB* FenceB::Clone() const
{
	return new FenceB(*this);
}

void FenceB::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldFenceB");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"FenceB_0Material", 0);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void FenceB::Update()
{
	GameObj::Update();
}

void FenceB::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void FenceB::OnCollisionEnter(GameObj* gameObject)
{
}

void FenceB::OnTriggerEnter(GameObj* gameObject)
{
}

void FenceB::OnTriggerPersist(GameObj* gameObject)
{
}

void FenceB::OnTriggerExit(GameObj* gameObject)
{
}

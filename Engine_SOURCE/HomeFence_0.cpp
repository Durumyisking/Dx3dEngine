#include "HomeFence_0.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeFence_0::HomeFence_0()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "HomeFence_0";
}

HomeFence_0::HomeFence_0(const HomeFence_0& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

HomeFence_0::~HomeFence_0()
{
}

HomeFence_0* HomeFence_0::Clone() const
{
	return new HomeFence_0(*this);
}

void HomeFence_0::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeFence000");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"Fence000_0Material", 0);
	mr->SetMaterialByKey(L"Fence000_1Material", 1);
	mr->SetMaterialByKey(L"Fence000_2Material", 2);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	//Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	//physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 01.f, 0.1f, 0.1f });

	//PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	//AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeFence_0::Update()
{
	GameObj::Update();
}

void HomeFence_0::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeFence_0::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeFence_0::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeFence_0::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeFence_0::OnTriggerExit(GameObj* gameObject)
{
}

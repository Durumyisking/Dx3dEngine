#include "CityWorldChairB.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


CityWorldChairB::CityWorldChairB()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "CityWorldChairB";
}

CityWorldChairB::CityWorldChairB(const CityWorldChairB& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

CityWorldChairB::~CityWorldChairB()
{

}

CityWorldChairB* CityWorldChairB::Clone() const
{
	return new CityWorldChairB(*this);
}

void CityWorldChairB::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldChairB");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CityWorldBushB_0Material");

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void CityWorldChairB::Update()
{
	GameObj::Update();
}

void CityWorldChairB::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityWorldChairB::OnCollisionEnter(GameObj* gameObject)
{
}

void CityWorldChairB::OnTriggerEnter(GameObj* gameObject)
{
}

void CityWorldChairB::OnTriggerPersist(GameObj* gameObject)
{
}

void CityWorldChairB::OnTriggerExit(GameObj* gameObject)
{
}

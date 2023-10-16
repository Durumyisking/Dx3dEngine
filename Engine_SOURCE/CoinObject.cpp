#include "CoinObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CoinObject::CoinObject()
{

}

CoinObject::~CoinObject()
{

}

void CoinObject::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Coin");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CoinObjectMaterial");

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	this->GetComponent<Transform>()->SetPhysicalRotation(Vector3(90.f, 0.f, 0.f));

	GameObj::Initialize();
}

void CoinObject::Update()
{
	GameObj::Update();
}

void CoinObject::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CoinObject::OnCollisionEnter(GameObj* gameObject)
{
}

void CoinObject::OnTriggerEnter(GameObj* gameObject)
{
}

void CoinObject::OnTriggerPersist(GameObj* gameObject)
{
}

void CoinObject::OnTriggerExit(GameObj* gameObject)
{
}

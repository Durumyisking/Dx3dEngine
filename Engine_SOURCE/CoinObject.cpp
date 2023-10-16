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

	mr->SetMaterialByKey(L"CoinMaterial",0);

	//this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Capsule, { 5.f, 5.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);



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

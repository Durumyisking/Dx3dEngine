#include "CityMapNaviObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


CityMapNaviObject::CityMapNaviObject()
{
}

CityMapNaviObject::~CityMapNaviObject()
{
}

void CityMapNaviObject::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"NaviCollider");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"NaviCollider_0Material", 0);
	mr->SetMaterialByKey(L"NaviCollider_1Material", 1);
	mr->SetMaterialByKey(L"NaviCollider_2Material", 2);
	mr->SetMaterialByKey(L"NaviCollider_3Material", 3);
	mr->SetMaterialByKey(L"NaviCollider_4Material", 4);
	mr->SetMaterialByKey(L"NaviCollider_5Material", 5);
	mr->SetMaterialByKey(L"NaviCollider_6Material", 6);
	mr->SetMaterialByKey(L"NaviCollider_7Material", 7);
	mr->SetMaterialByKey(L"NaviCollider_8Material", 8);
	mr->SetMaterialByKey(L"NaviCollider_9Material", 9);
	mr->SetMaterialByKey(L"NaviCollider_10Material", 10);
	mr->SetMaterialByKey(L"NaviCollider_11Material", 11);
	mr->SetMaterialByKey(L"NaviCollider_12Material", 12);
	mr->SetMaterialByKey(L"NaviCollider_13Material", 13);
	mr->SetMaterialByKey(L"NaviCollider_13Material", 14);
	mr->SetMaterialByKey(L"NaviCollider_14Material", 15);
	mr->SetMaterialByKey(L"NaviCollider_15Material", 16);
	mr->SetMaterialByKey(L"NaviCollider_16Material", 17);
	mr->SetMaterialByKey(L"NaviCollider_17Material", 18);
	mr->SetMaterialByKey(L"NaviCollider_18Material", 19);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);



	GameObj::Initialize();
}

void CityMapNaviObject::Update()
{
	GameObj::Update();
}

void CityMapNaviObject::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityMapNaviObject::OnCollisionEnter(GameObj* gameObject)
{
}

void CityMapNaviObject::OnTriggerEnter(GameObj* gameObject)
{
}

void CityMapNaviObject::OnTriggerPersist(GameObj* gameObject)
{
}

void CityMapNaviObject::OnTriggerExit(GameObj* gameObject)
{
}

#include "CreateObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CreateObject::CreateObject(const std::wstring& modelKey, std::vector<std::wstring> array)
	:MaterialArray(array)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(modelKey);
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	for (size_t i = 0; i < MaterialArray.size(); i++)
	{
		mr->SetMaterialByKey(MaterialArray[i], i);
	}
}

CreateObject::~CreateObject()
{
}

void CreateObject::Initialize()
{
	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void CreateObject::Update()
{
	GameObj::Update();
}

void CreateObject::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CreateObject::OnCollisionEnter(GameObj* gameObject)
{
}

void CreateObject::OnTriggerEnter(GameObj* gameObject)
{
}

void CreateObject::OnTriggerPersist(GameObj* gameObject)
{
}

void CreateObject::OnTriggerExit(GameObj* gameObject)
{
}

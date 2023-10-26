#include "CreateObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CreateObject::CreateObject(const std::wstring& modelKey,const std::wstring& materialKey,Vector3 size)
	:GameObj()
	,mModelKey(modelKey)
	,mMaterialKey(materialKey)
	,mColliderSize(size)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = std::string().assign(modelKey.begin(),modelKey.end());
}

CreateObject::CreateObject(const CreateObject& Obj)
	:GameObj(Obj)
	, mModelKey(Obj.mModelKey)
	, mMaterialKey(Obj.mMaterialKey)
	, mColliderSize(Obj.mColliderSize)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

CreateObject::~CreateObject()
{
}

CreateObject* CreateObject::Clone() const
{
	return  new CreateObject(*this);
}

void CreateObject::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelKey);
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterial(mMaterialKey);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

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

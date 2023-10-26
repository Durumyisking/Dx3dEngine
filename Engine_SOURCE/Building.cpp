#include "Building.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

Building::Building()
	: GameObj()
	, mModelName(L"CityWorldHomeBuilding002")
	, mbPhysical(true)
	, mPhysicalScale(10.f, 50.f, 10.f)
	, mGeometryType(eGeometryType::Box)
	, mActorType(eActorType::Static)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "Building";
}
Building::Building(const Building& Obj)
	:GameObj(Obj)
	, mModelName(Obj.mModelName)
	, mbPhysical(Obj.mbPhysical)
	, mPhysicalScale(Obj.mPhysicalScale)
	, mGeometryType(Obj.mGeometryType)
	, mActorType(Obj.mActorType)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

}
Building::~Building()
{

}

Building* Building::Clone() const
{
	return new Building(*this);
}

void Building::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetMaterial(model->GetMaterial());
	mr->SetModel(model);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	if (mbPhysical)
	{
		Physical* physical = AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(mActorType, mGeometryType, mPhysicalScale);

		PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

		AddComponent<PhysXCollider>(eComponentType::Collider);
	}

	GameObj::Initialize();
}

void Building::Update()
{
	GameObj::Update();
}

void Building::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Building::Render()
{
	GameObj::Render();
}

void Building::OnCollisionEnter(GameObj* gameObject)
{
}

void Building::OnTriggerEnter(GameObj* gameObject)
{
}

void Building::OnTriggerPersist(GameObj* gameObject)
{
}

void Building::OnTriggerExit(GameObj* gameObject)
{
}

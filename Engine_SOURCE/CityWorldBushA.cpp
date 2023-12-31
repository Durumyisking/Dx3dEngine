#include "CityWorldBushA.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

CityWorldBushA::CityWorldBushA()
	: GameObj()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "CityWorldBushA";
}

CityWorldBushA::CityWorldBushA(const CityWorldBushA& Obj)
	:GameObj(Obj)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

CityWorldBushA::~CityWorldBushA()
{
}

CityWorldBushA* CityWorldBushA::Clone() const
{
	return new CityWorldBushA(*this);
}

void CityWorldBushA::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldBushA");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"CityWorldBushA_0Material", 0);
	mr->SetMaterialByKey(L"CityWorldBushA_1Material", 1);
	mr->SetMaterialByKey(L"CityWorldBushA_0Material", 2);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 1.f, 1.f, 1.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);



	GameObj::Initialize();
}

void CityWorldBushA::Update()
{
	GameObj::Update();
}

void CityWorldBushA::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void CityWorldBushA::OnCollisionEnter(GameObj* gameObject)
{
}

void CityWorldBushA::OnTriggerEnter(GameObj* gameObject)
{
}

void CityWorldBushA::OnTriggerPersist(GameObj* gameObject)
{
}

void CityWorldBushA::OnTriggerExit(GameObj* gameObject)
{
}

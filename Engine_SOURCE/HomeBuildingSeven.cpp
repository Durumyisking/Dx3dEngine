#include "HomeBuildingSeven.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

HomeBuildingSeven::HomeBuildingSeven()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "HomeBuildingSeven";
}

HomeBuildingSeven::HomeBuildingSeven(const HomeBuildingSeven& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

HomeBuildingSeven::~HomeBuildingSeven()
{
}

HomeBuildingSeven* HomeBuildingSeven::Clone() const
{
	return new HomeBuildingSeven(*this);
}

void HomeBuildingSeven::Initialize()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding007");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"HomeBuilding007_0Material", 0);
	mr->SetMaterialByKey(L"HomeBuilding007_1Material", 1);
	mr->SetMaterialByKey(L"HomeBuilding007_0Material", 2);  //수정 필요 GlassBuilding1F01
	mr->SetMaterialByKey(L"HomeBuilding007_1Material", 3); 
	mr->SetMaterialByKey(L"HomeBuilding007_2Material", 4);
	mr->SetMaterialByKey(L"HomeBuilding007_3Material", 5);
	mr->SetMaterialByKey(L"HomeBuilding007_4Material", 6);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	GameObj::Initialize();
}

void HomeBuildingSeven::Update()
{
	GameObj::Update();
}

void HomeBuildingSeven::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void HomeBuildingSeven::OnCollisionEnter(GameObj* gameObject)
{
}

void HomeBuildingSeven::OnTriggerEnter(GameObj* gameObject)
{
}

void HomeBuildingSeven::OnTriggerPersist(GameObj* gameObject)
{
}

void HomeBuildingSeven::OnTriggerExit(GameObj* gameObject)
{
}

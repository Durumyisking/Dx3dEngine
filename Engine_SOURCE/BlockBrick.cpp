#include "BlockBrick.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"


BlockBrick::BlockBrick()
{
}

BlockBrick::~BlockBrick()
{
}

void BlockBrick::Initialize()
{
	SetScale(Vector3(1.f, 1.f, 1.f));

	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"BlockBrick");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"BlockBrickMaterial", 0);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 0.5f, 0.5f, 0.5f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);
	GameObj::Initialize();
}

void BlockBrick::Update()
{
	GameObj::Update();
}

void BlockBrick::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void BlockBrick::OnCollisionEnter(GameObj* gameObject)
{
}

void BlockBrick::OnTriggerEnter(GameObj* gameObject)
{
}

void BlockBrick::OnTriggerPersist(GameObj* gameObject)
{
}

void BlockBrick::OnTriggerExit(GameObj* gameObject)
{
}

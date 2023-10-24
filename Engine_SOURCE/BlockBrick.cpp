#include "BlockBrick.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "InstancingContainer.h"

BlockBrick::BlockBrick()
{
	SetName(L"BlockBrick");
	mObjectTypeName = "BlockBrick";
}

BlockBrick::BlockBrick(const BlockBrick& Obj)
	: InstantiativeObject(Obj)
{
}

BlockBrick::~BlockBrick()
{
}

BlockBrick* BlockBrick::Clone() const
{
	return new BlockBrick(*this);
}

void BlockBrick::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void BlockBrick::Load(FILE* File)
{
	DynamicObject::Load(File);
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
	InstantiativeObject::Initialize();
}

void BlockBrick::Update()
{
	InstantiativeObject::Update();
}

void BlockBrick::FixedUpdate()
{
	InstantiativeObject::FixedUpdate();
}

void BlockBrick::Render()
{
	InstantiativeObject::Render();
}

void BlockBrick::PrevRender()
{
	InstantiativeObject::PrevRender();
}

void BlockBrick::FontRender()
{
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

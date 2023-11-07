#include "BlockBrick.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "InstancingContainer.h"

#include "BoneAnimator.h"
#include "BlockBrickScript.h"

BlockBrick::BlockBrick()
	:mbInst(false)
{
	SetName(L"BlockBrick");
	mObjectTypeName = "BlockBrick";
	AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<BlockBrickScript>(eComponentType::Script);
}

BlockBrick::BlockBrick(const BlockBrick& Obj)
	: InstantiativeObject(Obj)
	, mbInst(false)

{
	AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	AddComponent<BlockBrickScript>(eComponentType::Script);
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

	BoneAnimator* animator = this->GetComponent<BoneAnimator>();

	animator->CreateAnimation(L"ReactionHipDrop", L"..//..//Resources/brick/Animation/ReactionHipDrop.smd", 0.05f);

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"BlockBrick");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	if (mbInst)
	{
		mr->SetMaterialByKey(L"BlockBrickMaterial_inst", 0);
	}
	else
	{
		mr->SetMaterialByKey(L"BlockBrickMaterial", 0);
	}

	this->GetComponent<Transform>()->SetOffsetScale(0.1f);

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

#include "PowerMoonObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "InstancingContainer.h"

#include "MoonScript.h"
#include "InputMgr.h"


PowerMoonObject::PowerMoonObject()
	:mbHide(false)

{
	SetName(L"PowerMoonObject");
	mObjectTypeName = "PowerMoonObject";
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

PowerMoonObject::PowerMoonObject(const PowerMoonObject& Obj)
	: InstantiativeObject(Obj)
	,mbHide(Obj.mbHide)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

PowerMoonObject::~PowerMoonObject()
{
}

PowerMoonObject* PowerMoonObject::Clone() const
{
	return new PowerMoonObject(*this);
}

void PowerMoonObject::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void PowerMoonObject::Load(FILE* File)
{
	DynamicObject::Load(File);
}

void PowerMoonObject::Initialize()
{
	SetScale(Vector3(1.f, 1.f, 1.f));
	SetRotation(Vector3(90.f, 0.f, 90.f));

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Shine");

	if (model == nullptr)
		return;

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"PowerMoon_0Material", 0);
	mr->SetMaterialByKey(L"PowerMoon_0Material", 1);

	this->GetComponent<Transform>()->SetOffsetScale(0.01f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Box, { 0.5f, 0.5f, 0.5f });


	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	mbHide = true;

	AddComponent<MoonScript>(eComponentType::Script);

	InstantiativeObject::Initialize();
}

void PowerMoonObject::Update()
{
	if (KEY_TAP(F_9))
	{
		GetScript<MoonScript>()->PowerMoonAppears();
	}

	InstantiativeObject::Update();
}

void PowerMoonObject::FixedUpdate()
{
	InstantiativeObject::FixedUpdate();
}

void PowerMoonObject::Render()
{
	if (mbHide)
		return;

	InstantiativeObject::Render();
}

void PowerMoonObject::PrevRender()
{
	if (mbHide)
		return;

	InstantiativeObject::PrevRender();
}

void PowerMoonObject::FontRender()
{
}

void PowerMoonObject::OnCollisionEnter(GameObj* gameObject)
{
}

void PowerMoonObject::OnTriggerEnter(GameObj* gameObject)
{
}

void PowerMoonObject::OnTriggerPersist(GameObj* gameObject)
{
}

void PowerMoonObject::OnTriggerExit(GameObj* gameObject)
{
}
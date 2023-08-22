#include "RayObject.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

RayObject::RayObject()
	: GameObj()
	, mDir()
	, mPickObj(nullptr)
{
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Capsule, Vector3(1.f, GetScale().y, 1.f));

	GetComponent<Transform>()->SetRotation(Vector3::Forward);
}

RayObject::RayObject(Vector3 mousePos, Vector3 camDir)
	: GameObj()
	, mDir(camDir)
	, mPickObj(nullptr)
{
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Static, eGeometryType::Capsule, Vector3(1.f, GetScale().y, 1.f));

	GetComponent<Transform>()->SetPosition(mousePos);
	GetComponent<Transform>()->SetRotation(camDir);
}

RayObject::~RayObject()
{
}

void RayObject::Initialize()
{
	GameObj::Initialize();
}

void RayObject::Update()
{
	GameObj::Initialize();
}

void RayObject::FixedUpdate()
{
	GameObj::Initialize();
}

void RayObject::Render()
{
	GameObj::Initialize();
}

void RayObject::FontRender()
{
	GameObj::Initialize();	
}

void RayObject::OnCollisionEnter(GameObj* gameObject)
{
	if (gameObject == nullptr)
		return;

	if (mPickObj != nullptr)
	{
		Vector3 pos = GetComponent<Transform>()->GetPosition();
		Vector3 pickPos = mPickObj->GetComponent<Transform>()->GetPosition();
		Vector3 objPos = gameObject->GetComponent<Transform>()->GetPosition();

		float pickDistance = Vector3::Distance(pos, pickPos);
		float objDistance = Vector3::Distance(pos, objPos);

		if (pickDistance > objDistance)
			mPickObj = gameObject;
	}
	else
		mPickObj = gameObject;
}

void RayObject::OnTriggerEnter(GameObj* gameObject)
{

}

void RayObject::OnTriggerExit(GameObj* gameObject)
{
	if (mPickObj == gameObject)
		mPickObj == nullptr;
}

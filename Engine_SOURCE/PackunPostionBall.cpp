#include "PackunPostionBall.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"
#include "Model.h"
#include "Physical.h"
#include "PhysicalMovement.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

PackunPostionBall::PackunPostionBall()
{
}

PackunPostionBall::~PackunPostionBall()
{
}

void PackunPostionBall::Initialize()
{
	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	if (!meshRenderer)
		return;

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"PackunBall");
	if (model)
	{
		// Model Setting
		meshRenderer->SetModel(model);

		//Model Default Material
		meshRenderer->SetMaterial(model->GetMaterial(0));
	}

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(0.5f, 0.5f, 0.5f));


	// Rigidbody
	PhysXRigidBody* rigidbody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigidbody->Initialize();

	// MoveMent
	AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();
	AddComponent<PhysicalMovement>(eComponentType::Movement)->Initialize();

	//test
	//GetComponent<Transform>()->SetPhysicalPosition(Vector3(-500.f, 0.0f, 1.0f));
}

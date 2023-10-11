#include "PhysicalGameObj.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"



PhysicalGameObj::PhysicalGameObj()
{
	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);
}
PhysicalGameObj::~PhysicalGameObj()
{
}
void PhysicalGameObj::Initialize()
{
	
	GameObj::Initialize();
}
void PhysicalGameObj::Update()
{
	GameObj::Update();
}
void PhysicalGameObj::FixedUpdate()
{
	GameObj::FixedUpdate();
}
void PhysicalGameObj::Render()
{ 
	GameObj::Render();
}
void PhysicalGameObj::FontRender()
{
	GameObj::FontRender();
}

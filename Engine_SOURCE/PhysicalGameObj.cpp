#include "PhysicalGameObj.h"
#include "GameObj.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"



PhysicalGameObj::PhysicalGameObj()
	: mPhysicalScale()
{
	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	mObjectTypeName = "PhysicalGameObj";
}
PhysicalGameObj::PhysicalGameObj(const PhysicalGameObj& Obj)
	: GameObj(Obj)
	, mPhysicalScale(Obj.mPhysicalScale)
{
	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::Renderer);

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	//rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);
}
PhysicalGameObj::~PhysicalGameObj()
{
}

PhysicalGameObj* PhysicalGameObj::Clone() const
{
	return new PhysicalGameObj(*this);
}

void PhysicalGameObj::Save(FILE* File)
{
	GameObj::Save(File);

	MeshRenderer* meshRenderer = GetComponent<MeshRenderer>();

	std::wstring mesh = meshRenderer->GetMeshKey();
	std::wstring material = meshRenderer->GetMaterialKey();


	int numWChars = (int)mesh.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(mesh.c_str(), sizeof(wchar_t), numWChars, File);


	numWChars = (int)material.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(material.c_str(), sizeof(wchar_t), numWChars, File);
}
void PhysicalGameObj::Load(FILE* File)
{
	GameObj::Load(File);

	MeshRenderer* meshRenderer = GetComponent<MeshRenderer>();

	std::wstring meshKey;
	std::wstring materialKey;

	int numWChars = 0;

	fread(&numWChars, sizeof(int), 1, File);

	meshKey.resize(numWChars);
	fread(&meshKey[0], sizeof(wchar_t), numWChars, File);

	fread(&numWChars, sizeof(int), 1, File);

	materialKey.resize(numWChars);
	fread(&materialKey[0], sizeof(wchar_t), numWChars, File);
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

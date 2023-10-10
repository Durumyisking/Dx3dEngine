#include "Box.h"
#include "Physical.h"



Box::Box()
{
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
	GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, GetScale() * 0.5f);

	mObjectTypeName = "Box";
}

Box::Box(const Box& Obj)
	: PhysicalGameObj(Obj)
{
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
	GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, GetScale() * 0.5f);
}

Box::~Box()
{
}

Box* Box::Clone() const
{
	return new Box(*this);
}

void Box::Save(FILE* File)
{
	PhysicalGameObj::Save(File);
}

void Box::Load(FILE* File)
{
	PhysicalGameObj::Load(File);
}

void Box::Initialize()
{
	PhysicalGameObj::Initialize();
}

void Box::Update()
{
	PhysicalGameObj::Update();
}

void Box::FixedUpdate()
{
	PhysicalGameObj::FixedUpdate();
}

void Box::Render()
{
	PhysicalGameObj::Render();
}

void Box::FontRender()
{
	PhysicalGameObj::FontRender();
}

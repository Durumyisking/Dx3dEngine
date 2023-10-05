#include "Sphere.h"
#include "Physical.h"



Sphere::Sphere()
	: PhysicalGameObj()
{
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
	GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");	

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, GetScale() * 0.5f);


	mObjectTypeName = "Sphere";
}

Sphere::Sphere(const Sphere& Obj)
	: PhysicalGameObj(Obj)
{
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
	GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, GetScale() * 0.5f);
}

Sphere::~Sphere()
{
}

Sphere* Sphere::Clone() const
{
	return new Sphere(*this);
}

void Sphere::Save(FILE* File)
{
	PhysicalGameObj::Save(File);
}

void Sphere::Load(FILE* File)
{
	PhysicalGameObj::Load(File);
}

void Sphere::Initialize()
{
	PhysicalGameObj::Initialize();
}

void Sphere::Update()
{
	PhysicalGameObj::Update();
}

void Sphere::FixedUpdate()
{

	PhysicalGameObj::FixedUpdate();
}

void Sphere::Render()
{
	PhysicalGameObj::Render();
}

void Sphere::FontRender()
{
	PhysicalGameObj::FontRender();
}

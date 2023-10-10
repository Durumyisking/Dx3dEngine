#include "Sphere.h"
#include "Physical.h"



Sphere::Sphere()
{
	GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
	GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");	

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, GetScale() * 0.5f);
}

Sphere::~Sphere()
{
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

#include "Box.h"
#include "Physical.h"

namespace dru
{
	Box::Box()
	{
		GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");

		Physical* physical = AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, GetScale() * 0.5f);

	}

	Box::~Box()
	{
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

}
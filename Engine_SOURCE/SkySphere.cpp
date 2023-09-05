#include "SkySphere.h"
#include "SkySphereRenderer.h"

SkySphere::SkySphere()
{
    SkySphereRenderer* sr =  AddComponent<SkySphereRenderer>(eComponentType::MeshRenderer);
    sr->SetMaterialByKey(L"SkySphereMaterial");
    sr->SetMeshByKey(L"Spheremesh");
}

SkySphere::~SkySphere()
{
}

void SkySphere::Initialize()
{
    GameObj::Initialize();
}

void SkySphere::Update()
{
    GameObj::Update();
}

void SkySphere::FixedUpdate()
{
    GameObj::FixedUpdate();
}

void SkySphere::Render()
{
    GameObj::Render();
}

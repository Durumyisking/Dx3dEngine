#include "SkySphere.h"
#include "SkySphereRenderer.h"

SkySphere::SkySphere()
    : GameObj()
{
    mObjectTypeName = "SkySphere";
}

SkySphere::SkySphere(const SkySphere& Obj)
    : GameObj(Obj)
{
}

SkySphere::~SkySphere()
{
}

SkySphere* SkySphere::Clone() const
{
    return new SkySphere(*this);
}

void SkySphere::Save(FILE* File)
{
    GameObj::Save(File);
}

void SkySphere::Load(FILE* File)
{
    GameObj::Load(File);
}

void SkySphere::Initialize()
{
    SkySphereRenderer* sr = AddComponent<SkySphereRenderer>(eComponentType::MeshRenderer);
    sr->SetMaterialByKey(L"SkySphereMaterial");
    sr->SetMeshByKey(L"Spheremesh");

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

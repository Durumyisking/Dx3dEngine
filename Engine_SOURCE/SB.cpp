#include "SB.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"
#include "CubeMap.h"

ScreenQuad::ScreenQuad()
{
}

ScreenQuad::~ScreenQuad()
{
}

void ScreenQuad::Draw()
{

}

Skybox::Skybox()
	:mMesh(nullptr)
{
	mMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

}

Skybox::~Skybox()
{
}

void Skybox::Draw()
{
	mMesh->BindBuffer();
	mMesh->Render();
}

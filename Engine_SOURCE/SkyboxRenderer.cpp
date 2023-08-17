#include "SkyboxRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"

SkyboxRenderer::SkyboxRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
{
	SetMaterialByKey(L"SkyBoxMaterial");
}

SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::Initialize()
{
	BaseRenderer::Initialize();
}

void SkyboxRenderer::Update()
{
	BaseRenderer::Update();
}

void SkyboxRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void SkyboxRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial()->GetShader()->Bind();

	// draw
	GetMesh()->BindBuffer();
	GetMesh()->Render();

	GetMaterial()->Clear();

	//Texture::Clear(12);
	//Texture::Clear(13);
	//Texture::Clear(14);
}

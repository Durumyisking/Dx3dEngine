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

	// 텍스처는 cubemap에서 바인딩
	GetMaterial()->GetShader()->Bind();

	// draw
	GetMesh()->BindBuffer();
	GetMesh()->Render();

	GetMaterial()->Clear();

}

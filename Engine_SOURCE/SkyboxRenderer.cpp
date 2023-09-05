#include "SkyboxRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"

SkyboxRenderer::SkyboxRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
	, mProjConstantBuffer{}
{

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

	// skysphere �ؽ�ó ���ε�.
	GetMaterial()->Bind();

	// draw
	GetMesh()->BindBuffer();
	GetMesh()->Render();

	GetMaterial()->Clear();

}

void SkyboxRenderer::SetSkyBoyTextureByKey(const std::wstring& texureName)
{
	GetMaterial()->SetTextureByKey(texureName, eTextureSlot::SkySphere);
}

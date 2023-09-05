#include "SkySphereRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"

SkySphereRenderer::SkySphereRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
	, mProjConstantBuffer{}
{

}

SkySphereRenderer::~SkySphereRenderer()
{
}

void SkySphereRenderer::Initialize()
{
	BaseRenderer::Initialize();

}

void SkySphereRenderer::Update()
{
	BaseRenderer::Update();
}

void SkySphereRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void SkySphereRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	// skysphere 텍스처 바인딩.
	GetMaterial()->Bind();

	// draw
	GetMesh()->BindBuffer();
	GetMesh()->Render();

	GetMaterial()->Clear();

}

void SkySphereRenderer::SetSkyBoyTextureByKey(const std::wstring& texureName)
{
	GetMaterial()->SetTextureByKey(texureName, eTextureSlot::SkySphere);
}

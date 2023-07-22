#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"



MeshRenderer::MeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialize()
{
	BaseRenderer::Initialize();
}

void MeshRenderer::Update()
{
	BaseRenderer::Update();
}

void MeshRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void MeshRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial()->Bind();
	GetMesh()->BindBuffer();

	GetMesh()->Render();

	GetMaterial()->Clear();
}

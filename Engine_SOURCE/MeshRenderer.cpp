#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"



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

	if (GetModel() != nullptr)
	{
		GetModel()->Bind_Render();
	}
	else
	{
		GetMaterial()->Bind();
		GetMesh()->BindBuffer();
		GetMesh()->Render();
		GetMaterial()->Clear();
	}
}

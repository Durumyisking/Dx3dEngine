#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"

namespace dru
{
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

		if (nullptr != GetModel())
		{
			GetModel()->Bind();
			GetModel()->Render();
		}
		else
		{
			GetMesh()->BindBuffer();
			GetMesh()->Render();
		}

		GetMaterial()->Clear();
	}
}
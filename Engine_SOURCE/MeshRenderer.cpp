#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"

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

	void MeshRenderer::update()
	{
		BaseRenderer::update();
	}

	void MeshRenderer::fixedUpdate()
	{
		BaseRenderer::fixedUpdate();
	}

	void MeshRenderer::render()
	{
		BaseRenderer::render();

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMaterial()->Bind();
		GetMesh()->BindBuffer();

		GetMesh()->Render();

		GetMaterial()->Clear();
	}
}
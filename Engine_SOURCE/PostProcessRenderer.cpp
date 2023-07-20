#include "PostProcessRenderer.h"
#include "PostProcess.h"
#include "Material.h"

namespace dru
{
	PostProcessRenderer::PostProcessRenderer()
		: BaseRenderer(eComponentType::Renderer)
	{
	}

	PostProcessRenderer::~PostProcessRenderer()
	{
	}

	void PostProcessRenderer::Initialize()
	{
		BaseRenderer::Initialize();
	}

	void PostProcessRenderer::Update()
	{
		BaseRenderer::Update();
	}

	void PostProcessRenderer::FixedUpdate()
	{
		BaseRenderer::FixedUpdate();
	}

	void PostProcessRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMesh()->BindBuffer();

		mPostProcess->Bind();
		GetMaterial()->Bind();

		GetMesh()->Render();

		mPostProcess->Clear();
		GetMaterial()->Clear();

		BaseRenderer::Render();
	}

	void PostProcessRenderer::SetPostProcessOwner(PostProcess* postProcess)
	{
		mPostProcess = postProcess;
	}
}
#include "PostProcessRenderer.h"
#include "PostProcess.h"

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

	void PostProcessRenderer::update()
	{
		BaseRenderer::update();
	}

	void PostProcessRenderer::fixedUpdate()
	{
		BaseRenderer::fixedUpdate();
	}

	void PostProcessRenderer::render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMesh()->BindBuffer();

		mPostProcess->Bind();
		GetMaterial()->Bind();

		GetMesh()->Render();

		mPostProcess->Clear();
		GetMaterial()->Clear();

		BaseRenderer::render();
	}

	void PostProcessRenderer::SetPostProcessOwner(PostProcess* postProcess)
	{
		mPostProcess = postProcess;
	}
}
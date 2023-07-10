#include "PostProcess.h"
#include "TimeMgr.h"
#include "Application.h"
#include "PostProcessRenderer.h"

extern dru::Application application;

namespace dru
{
	PostProcess::PostProcess()
		: mPostProcessMaterial(nullptr)
		, mConstantBuffer{}
		, mRenderer(nullptr)
	{
		mConstantBuffer.Resolution = application.WinResolution();

		mRenderer = AddComponent<PostProcessRenderer>(eComponentType::Renderer);
		Material* mateiral = GETSINGLE(ResourceMgr)->Find<Material>(L"PostProcessMaterial");
		mRenderer->SetMaterial(mateiral);
		mRenderer->SetPostProcessOwner(this);

		SetPos(Vector3(0.f, 0.f, 0.f));
		SetScale(Vector3(16.0f, 9.0f, 1.0f));
	}

	PostProcess::~PostProcess()
	{
	}

	void PostProcess::Initialize()
	{
		GameObj::Initialize();
	}

	void PostProcess::update()
	{
		mConstantBuffer.ElapsedTime += DT;

		GameObj::update();
	}

	void PostProcess::fixedUpdate()
	{
		GameObj::fixedUpdate();
	}

	void PostProcess::render()
	{
		GameObj::render();
	}

	void PostProcess::Bind()
	{
		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::PostProcess)];
		cb->SetData(&mConstantBuffer);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);


	}

	void PostProcess::Clear()
	{
		ConstantBuffer* pCB = renderer::constantBuffers[static_cast<UINT>(eCBType::PostProcess)];
		pCB->Clear();
	}

	void PostProcess::SetMaterial(std::wstring _Key)
	{
		Material* mateiral = GETSINGLE(ResourceMgr)->Find<Material>(_Key);
		mRenderer->SetMaterial(mateiral);
	}

}